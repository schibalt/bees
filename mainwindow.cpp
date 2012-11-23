#include <QFuture>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <iostream>
#include <sstream>
#include <QtConcurrentRun>
#include "algorithmmodel.h"

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
}

void MainWindow::on_pushButton_clicked()
{
    QFuture<void> future = QtConcurrent::run(beeAlgModel, &AlgorithmModel::genesis, initialPop, fieldDims);
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
