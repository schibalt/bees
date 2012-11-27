
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _ui->stepButton->installEventFilter(this);
    _scene = new QGraphicsScene(this);
    _ui->graphicsView->setScene(_scene);
    _genCap = _ui->genCap->value();
    _sites = _ui->sites->value();
    _eliteSites = _ui->eliteSites->value();
    _initialPop = _ui->initialPop->value();
    _fieldDims = QSize(_ui->fieldWidth->value(), _ui->fieldHeight->value());
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
        cout << "change algorithm state" << endl;
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
    QImage image;
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
    setBackground();
    QFuture<void> future2 = QtConcurrent::run(this, &MainWindow::draw);
    future2.waitForFinished();
}

void MainWindow::on_pushButton_clicked()
{
    _ui->pushButton->setEnabled(false);
    _ui->stepButton->setEnabled(true);

    //threaded call to AlgModel that makes the bees
    QFuture<vector<Bee > > genesisCall = QtConcurrent::run(_beeAlgModel, &AlgorithmModel::genesis, _initialPop, _fieldDims, false);
    genesisCall.waitForFinished();
    _beeAlgModel.setBees(genesisCall.result());
    _beeAlgModel.setHive(_fieldDims);

    QFuture<double** > foxholeCall = QtConcurrent::run(_beeAlgModel, &AlgorithmModel::foxholes);
    foxholeCall.waitForFinished();

    //double** F = beeAlgModel.foxholes();
    double** F = foxholeCall.result();

    ofstream foxholeFilestream;
    char foxholeFilename[] = "foxholes.txt";
    foxholeFilestream.open(foxholeFilename, ios::out);

    for (int i = 0; i < _SHEKEL_DIMENSION; i++)
    {
        for (int j = 0; j < _SHEKEL_DIMENSION; j++)
            foxholeFilestream << F[i][j] << " ";
        foxholeFilestream << endl;
    }
    foxholeFilestream.close();

    setGraduation(F);
    generateContourMap(F);
    
    season.setSeasonLength(_ui->genCap->value());
    season.start();

    //threaded call to this object that draws the bees & the hive
    QFuture<void> drawCall = QtConcurrent::run(this, &MainWindow::draw);
    drawCall.waitForFinished();
}

void MainWindow::generateContourMap(double** foxholes)
{
    _beeAlgModel.computeField(foxholes);
    QImage contourMap
    (
        QSize(_fieldDims.width(), _fieldDims.height()),
        QImage::Format_RGB32
    );

    int m = _fieldDims.height();
    int n = _fieldDims.width();

    const double** field = _beeAlgModel.getField();

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
    writer.write(contourMap);
}

void MainWindow::setGraduation(double** foxholes)
{
    int lowerBound = foxholes[0][0];
    int upperBound = foxholes[0][0];

    for (int i = 0; i < _SHEKEL_DIMENSION; i++)
    {
        for (int j = 0; j < _SHEKEL_DIMENSION; j++)
        {
            if (foxholes[i][j] > upperBound)
                upperBound = foxholes[i][j];

            if (foxholes[i][j] < lowerBound)
                lowerBound = foxholes[i][j];
        }
    }
    _lowerBound = lowerBound;
    _upperBound = upperBound;

    int difference = upperBound - lowerBound;
    _graduation = difference / 10;
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
            switch (grade)
            {
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
    return qRgb(0, 0, 0);
}

void MainWindow::draw()
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

    setBackground();

    vector<Bee > bees = _beeAlgModel.getBees();
    foreach(Bee bee, bees)
    {
        //cout << "drawing a bee" << endl;
        QImage image;
        image.load("..\\beesmall.png");
        QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));

        double xRatio = (double) bee.getPoint().x() / _fieldDims.width();
        double yRatio = (double) bee.getPoint().y() / _fieldDims.height();

        int xPos = xRatio * _ui->graphicsView->width();
        int yPos = yRatio * _ui->graphicsView->height();

        if (xRatio < 0 || yRatio < 0 || xRatio >= 1 || yRatio >= 1)
            cout << "ratios wrong" << endl;

        //cout << "graphicsview is " << ui->graphicsView->width() << ", " << ui->graphicsView->height() << endl;
        //cout << "pos is " << xPos << ", " << yPos << endl;
        Qgpmi->setPos(xPos, yPos);
        (*_scene).addItem(Qgpmi);
    }
    Hive hive = _beeAlgModel.getHive();
    double xRatio = (double) hive.getPoint().x() / _fieldDims.width();
    double yRatio = (double) hive.getPoint().y() / _fieldDims.height();
    int xPos = xRatio * _ui->graphicsView->width();
    int yPos = yRatio * _ui->graphicsView->height();
    QImage image;
    image.load("..\\Bee_Hive.png");
    //image = image.scaled(213, 300);

    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    Qgpmi->setPos(xPos, yPos);
    (*_scene).addItem(Qgpmi);
}

void MainWindow::on_genCap_valueChanged(int newGenCap)
{
    cout << "gencap is " << _genCap;
    _genCap = newGenCap;
    cout << " and is now " << _genCap << " and should be " << newGenCap << endl;
}

void MainWindow::on_initialPop_valueChanged(int newInitPop)
{
    cout << "initialPop is " << _initialPop;
    _initialPop = newInitPop;
    cout << " and is now " << _initialPop << " and should be " << newInitPop << endl;
}

void MainWindow::on_fieldWidth_valueChanged(int)
{
    int tempFieldWidth = _ui->fieldWidth->value();

    /*
    while(tempFieldWidth % (_SHEKEL_DIMENSION - 1) != 0)
        tempFieldWidth++;

    _ui->fieldWidth->setValue(tempFieldWidth);
    */
    _fieldDims.setWidth(tempFieldWidth);
}

void MainWindow::on_fieldHeight_valueChanged(int)
{
    int tempFieldHeight = _ui->fieldHeight->value();

    /*
    while(tempFieldHeight % (_SHEKEL_DIMENSION - 1) != 0)
        tempFieldHeight++;

    _ui->fieldHeight->setValue(tempFieldHeight);
    */
    _fieldDims.setHeight(tempFieldHeight);
}
