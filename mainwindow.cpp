
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    genCap = ui->genCap->value();
    sites = ui->sites->value();
    eliteSites = ui->eliteSites->value();
    initialPop = ui->initialPop->value();
    fieldDims = QSize(ui->fieldWidth->value(), ui->fieldHeight->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
bool MainWindow::setBackground()
{
    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }
    QImage image;
    image.load("..\\grass_field_stock_by_ymntle_stock-d32yw79half.jpg");

    int widthOffset = 2;
    int heightOffset = 4;

    //image = image.copy(0,0, ui->graphicsView->size().width(), ui->graphicsView->size().height());
    image = image.scaled(ui->graphicsView->size().width() - widthOffset, ui->graphicsView->size().height() - heightOffset);

    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    (*scene).addItem(Qgpmi);
    ui->graphicsView->setSceneRect(QRect(0, 0, image.width(), image.height()));

    return true;
}

void MainWindow::resizeEvent(QResizeEvent* event){
    setBackground();
    QFuture<void> future2 = QtConcurrent::run(this, &MainWindow::draw);
    future2.waitForFinished();
}

void MainWindow::on_pushButton_clicked()
{
    //beeAlgModel.genesis(initialPop, fieldDims);
    QFuture<vector<Bee > > future = QtConcurrent::run(beeAlgModel, &AlgorithmModel::genesis, initialPop, fieldDims, false);
    future.waitForFinished();
    beeAlgModel.setBees(future.result());
    beeAlgModel.setHive(fieldDims);
    QFuture<void> future2 = QtConcurrent::run(this, &MainWindow::draw);
    future2.waitForFinished();
}

void MainWindow::draw()
{
    // the items (lines and points) in the graphicsview
    QList<QGraphicsItem*> list = (*scene).items();

    // delete everything.  this is a new case.
    QList<QGraphicsItem*>::Iterator it = list.begin();
    for (; it != list.end(); ++it)
    {
        if (*it)
        {
            (*scene).removeItem(*it);
            delete *it;
        }
    }

    setBackground();

    vector<Bee > bees = beeAlgModel.getBees();
    foreach(Bee bee, bees)
    {
        //cout << "drawing a bee" << endl;
        QImage image;
        image.load("..\\beesmall.png");
        QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));

        double xRatio = (double) bee.getPoint().x() / fieldDims.width();
        double yRatio = (double) bee.getPoint().y() / fieldDims.height();

        int xPos = xRatio * ui->graphicsView->width();
        int yPos = yRatio * ui->graphicsView->height();

        if(xRatio < 0 || yRatio < 0 || xRatio >= 1 || yRatio >= 1)
            cout << "ratios wrong" << endl;

        //cout << "graphicsview is " << ui->graphicsView->width() << ", " << ui->graphicsView->height() << endl;
        //cout << "pos is " << xPos << ", " << yPos << endl;
        Qgpmi->setPos(xPos, yPos);
        (*scene).addItem(Qgpmi);
    }
    Hive hive = beeAlgModel.getHive();
    double xRatio = (double) hive.getPoint().x() / fieldDims.width();
    double yRatio = (double) hive.getPoint().y() / fieldDims.height();
    int xPos = xRatio * ui->graphicsView->width();
    int yPos = yRatio * ui->graphicsView->height();
    QImage image;
    image.load("..\\Bee_Hive.png");
    //image = image.scaled(213, 300);

    QGraphicsPixmapItem* Qgpmi = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    Qgpmi->setPos(xPos, yPos);
    (*scene).addItem(Qgpmi);
}

void MainWindow::on_genCap_valueChanged(int newGenCap)
{
    cout << "gencap is " << genCap;
    genCap = newGenCap;
    cout << " and is now " << genCap << " and should be " << newGenCap << endl;
}

void MainWindow::on_initialPop_valueChanged(int newInitPop)
{
    cout << "initialPop is " << initialPop;
    initialPop = newInitPop;
    cout << " and is now " << initialPop << " and should be " << newInitPop << endl;
}
