
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _ui->stepButton->installEventFilter(this);
    _scene = new QGraphicsScene(this);
    _ui->graphicsView->setScene(_scene);
    _okayToDraw = false;
    
    //when the thread finishes go to mapped slot
    QObject::connect(&_workerBee, SIGNAL(beesGenerated()), this, SLOT(beesGenerated()));
    QObject::connect(&_workerBee, SIGNAL(fieldGenerated()), this, SLOT(fieldGenerated()));
    QObject::connect(&_workerBee, SIGNAL(foxholesGenerated()), &_workerBee, SLOT(computeField()));
    QObject::connect(&_workerBee, SIGNAL(fitnessesEvaluated()), this, SLOT(fitnessesEvaluated()));
    
    //_workerBee.setConnections(_thread);
    _workerBee.moveToThread(&_thread);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            _ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        // Turn timer back on
        qDebug() << "change algorithm state" << endl;
    }
    return false;
}

bool MainWindow::setBackground()
{
    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*_scene).items();
    
    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*_scene).removeItem(*it);
            delete *it;
        }
    }

    bool useContourMap = true;
    QImage image;

    if (useContourMap)
        image = contourMap;
    else
        image.load("..\\grass_field_stock_by_ymntle_stock-d32yw79half.jpg");

    int widthOffset = 2;
    int heightOffset = 4;
    
    //image = image.copy(0,0, ui->graphicsView->size().width(), ui->graphicsView->size().height());
    image = image.scaled(_ui->graphicsView->size().width() - widthOffset, _ui->graphicsView->size().height() - heightOffset);
    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    (*_scene).addItem(Qgpmi);
    _ui->graphicsView->setSceneRect(QRect(0, 0, image.width(), image.height()));
    
    return true;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if (_okayToDraw)
    {
        //setBackground();
        QFuture<void> future2 = QtConcurrent::run(this, &MainWindow::initialDraw);
        future2.waitForFinished();
    }
}

void MainWindow::on_pushButton_clicked()
{
    _okayToDraw = true;
    //_ui->pushButton->setEnabled(false);
    _ui->stepButton->setEnabled(true);
    
    int initialPopulation = _ui->initialPop->value();
    int fieldWidth = _ui->fieldWidth->value();
    int fieldHeight = _ui->fieldHeight->value();
    QSize fieldDims = QSize(fieldWidth, fieldHeight);
    
    _workerBee.setGenesisMembers(_thread, initialPopulation, fieldDims, false);
    
    _thread.start();
}

void MainWindow::beesGenerated()
{
    qDebug() << "bees generated";
    
    int foxholes = _ui->foxholes->value();
    int maxima = _ui->maxima->value();
    int bound = _ui->bound->value();
    int power = _ui->power->value();
    
    _workerBee.setFieldGenMembers(_thread, foxholes, maxima, bound, power);
    
    qDebug() << "bees have been generated and";
    _thread.isRunning() ? qDebug() << "thread is running" : qDebug() << "thread isn't running";
    _thread.start();
}

void MainWindow::fieldGenerated()
{
    qDebug() << "field has been generated and";
    ofstream foxholeFilestream;
    char foxholeFilename[] = "foxholes.txt";
    foxholeFilestream.open(foxholeFilename, ios::out);
    
    int foxholeNumber = _ui->foxholes->value();
    const int foxholeMatDim = (2 * foxholeNumber) + 1;
    
    const double** F = _workerBee.getFoxholes();
    
    for (int i = 0; i < foxholeMatDim; i++)
    {
        for (int j = 0; j < foxholeMatDim; j++)
            foxholeFilestream << F[i][j] << " ";
        foxholeFilestream << endl;
    }
    foxholeFilestream.close();
    
    setGraduation(F);
    contourMap = generateContourMap(F);

    int sites = _ui->sites->value();
    int eliteSites = _ui->eliteSites->value();

    _thread.isRunning() ? qDebug() << "thread is running" : qDebug() << "thread isn't running";
    _workerBee.setFitnessEvalMembers(_thread, sites, eliteSites);
    _thread.start();
}

void MainWindow::fitnessesEvaluated()
{
    if (_ui->checkBox->isChecked())
    {
        initialDraw();

        //threaded call to this object that draws the bees & the hive
        // QFuture<void> drawCall = QtConcurrent::run(this, &MainWindow::initialDraw);
        //drawCall.waitForFinished();
    }
    else
    {
        //evaluate hive fitness
        _step = 1;
        _day = 0;
        nextStep();
    }
}

void MainWindow::nextStep()
{
    /*
      *run the step that has to be run
      *
      *before returning from the function decide whether or not to increment the day
      *and run the next step or draw what was just done and let the draw function
      *increment the day.
      */
    qDebug() << _day;
    
    int seasonLength = _ui->genCap->value();
    
    if (!_ui->checkBox->isChecked() && _day < seasonLength)
    {
        switch (_step)
        {
                /*select sites for neighborhood search.
                  iterate through the entire population and pick the
                  [site variable] best sites
                  */
            case 1:
                break;
                
                
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            default:
                qDebug();
        }
        
        ++_day;
        ++_step;
        nextStep();
    }
    else
        drawStep();
}

void MainWindow::drawStep()
{

}

QImage MainWindow::generateContourMap(const double** foxholes)
{
    WorkerBee workerBee;
    //workerBee.computeField(foxholes);
    
    int fieldWidth = _ui->fieldWidth->value();
    int fieldHeight = _ui->fieldHeight->value();
    //QSize fieldDims = QSize(fieldWidth, fieldHeight);
    
    QImage contourMap
    (
        QSize( fieldWidth,  fieldHeight),
         QImage::Format_RGB32
    );
    
    int m = fieldHeight;
    int n = fieldWidth;
    
    const double** field = _workerBee.getField();
    
    ofstream fieldFilestream;
    char fieldFilename[] = "fieldfromwindow.txt";
    fieldFilestream.open(fieldFilename, ios::out);
    
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fieldFilestream << field[i][j] << " ";
        }
        fieldFilestream << endl;
    }
    fieldFilestream.close();
    
    QRgb color;
    double value;
    
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            value = field[i][j];
            color = getColor(value);
            contourMap.setPixel(j, i, color);
        }
    }
    stringstream filename;
    
    filename << /*QDir::currentPath().toStdString() <<*/ "contourMap" << abs(rand() / 2) << ".jpg";
    QImageWriter writer(QString::fromStdString(filename.str()), "jpg");
    //writer.write(contourMap);
    //setBackground(contourMap);
    return contourMap;
}

void MainWindow::setGraduation(const double** foxholes)
{
    double lowerBound = foxholes[0][0];
    double upperBound = foxholes[0][0];

    const int foxholeMatDim = (_ui->foxholes->value() * 2) + 1;

    for (int i = 0; i < foxholeMatDim; i++)
    {
        for (int j = 0; j < foxholeMatDim; j++)
        {
            if (foxholes[i][j] > upperBound)
                upperBound = foxholes[i][j];

            if (foxholes[i][j] < lowerBound)
                lowerBound = foxholes[i][j];
        }
    }
    _lowerBound = lowerBound;
    _upperBound = upperBound;

    double difference = (double) upperBound - lowerBound;
    _graduation = (double) difference / _GRADES;
}

QRgb MainWindow::getColor(double value)
{
    bool isGrade = false;
    
    for (int grade = 1; grade <= _GRADES; grade++)
    {
        if (value < _lowerBound + _graduation * grade)
            isGrade = true;
            
        if (isGrade)
        {
            switch (grade - 1)
            {
                case 0:
                    return qRgb(0, 0, 0);
                    break;
                case 1:
                    return qRgb(127, 0, 255);
                    break;
                case 2:
                    return qRgb(0, 0, 255);
                    break;
                case 3:
                    return qRgb(0, 127, 255);
                    break;
                case 4:
                    return qRgb(0, 255, 255);
                    break;
                case 5:
                    return qRgb(0, 255, 127);
                    break;
                case 6:
                    return qRgb(0, 255, 0);
                    break;
                case 7:
                    return qRgb(127, 255, 0);
                    break;
                case 8:
                    return qRgb(255, 255, 0);
                    break;
                case 9:
                    return qRgb(255, 127, 0);
                    break;
                default:
                    return qRgb(255, 0, 0);
            }
        }
    }
    return qRgb(255, 255, 255);
}

void MainWindow::initialDraw()
{
    setBackground();
    
    int fieldWidth = _ui->fieldWidth->value();
    int fieldHeight = _ui->fieldHeight->value();

    Hive hive = _workerBee.getHive();

    double xRatio = (double) hive.getPoint().x() / fieldWidth;
    double yRatio = (double) hive.getPoint().y() / fieldHeight;
    int xPos = xRatio * _ui->graphicsView->width();
    int yPos = yRatio * _ui->graphicsView->height();
    QImage image;
    image.load("..\\Bee_Hive.png");
    //image = image.scaled(213, 300);

    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    Qgpmi->setPos(xPos, yPos);
    (*_scene).addItem(Qgpmi);
    
    const vector<Bee >* bees = _workerBee.getBees();
    foreach(Bee bee, *bees)
    {
        //qDebug() << "drawing a bee" << endl;
        QImage image;

        switch(bee.getRole())
        {
        case Bee::SCOUT:
            image.load("..\\copperbee.png");
            break;
        case Bee::RECRUIT:
            image.load("..\\silverbee.png");
            break;
        case Bee::PRIORITY:
            image.load("..\\goldbee.png");
            break;
        case Bee::ELITE:
            image.load("..\\diamondbee.png");
            break;
        default:
            qDebug() << "don't understand bee role";
        }

        QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        
        double xRatio = (double) bee.getPoint().x() / fieldWidth;
        double yRatio = (double) bee.getPoint().y() / fieldHeight;
        
        int xPos = xRatio * _ui->graphicsView->width();
        int yPos = yRatio * _ui->graphicsView->height();
        
        if (xRatio < 0 || yRatio < 0 || xRatio >= 1 || yRatio >= 1)
            qDebug() << "ratios wrong" << endl;
            
        //qDebug() << "graphicsview is " << ui->graphicsView->width() << ", " << ui->graphicsView->height() << endl;
        //qDebug() << "pos is " << xPos << ", " << yPos << endl;
        Qgpmi->setPos(xPos, yPos);
        (*_scene).addItem(Qgpmi);
    }
}

void MainWindow::on_stepButton_clicked()
{
    nextStep();
}
