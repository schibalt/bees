#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simulateseason.h"
//#include "algorithmmodel.h"
#include "ui_mainwindow.h"
#include "simulateseason.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include <QDir>
#include <QImageWriter>
#include <QImage>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QFuture>
#include <QGraphicsPixmapItem>
#include <QtConcurrentRun>
#include <QSignalMapper>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT
    public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent* e);

    private:
        Ui::MainWindow* _ui;
        QThread _thread;
        WorkerBee _workerBee;
        void resizeEvent(QResizeEvent* event);
        bool eventFilter(QObject* , QEvent*);

        QGraphicsScene* _scene;
        bool setBackground();
        void initialDraw();
        int _graduation;
        int _upperBound;
        int _lowerBound;
        QImage contourMap;
        const static int _GRADES = 11;

        void computeField();
        void setGraduation(const double**);
        QImage generateContourMap(const double**);
        QRgb getColor(double);
        bool _okayToDraw ;
        void drawNeighborhoodBoxes();
        void drawBox(vector<Bee* >);

        int _step;
        int _day;
        const static int _STEPS = 4;

        void disconnectEverything();

    public slots:
        void beesGenerated();
        void fieldGenerated();
        void fitnessesEvaluated();

        //main loop
        void nextStep();
        void drawStep();

    private slots:
        void on_stepButton_clicked();
        void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
