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
        void resizeEvent(QResizeEvent* event);
        void initialDraw();
        QImage generateContourMap(const double**);
        void setGraduation(const double**);
        void nextStep();
        QRgb getColor(double);
        bool eventFilter(QObject* , QEvent*);
        bool setBackground();
        void drawStep();
        void computeField();

        Ui::MainWindow* _ui;
        WorkerBee _workerBee;

        QGraphicsScene* _scene;
        QThread _thread;
        QImage contourMap;

        int _graduation;
        int _upperBound;
        int _lowerBound;
        int _step;
        int _day;
        int steps;

        const static int _grades = 11;
        bool _okayToDraw ;

    public slots:
        void beesGenerated();
        void fieldGenerated();


    private slots:
        /*
                void on_fieldHeight_valueChanged(int);
                void on_fieldWidth_valueChanged(int);
                void on_initialPop_valueChanged(int);
                void on_genCap_valueChanged(int);
        */
        void on_stepButton_clicked();
        void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
