#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simulateseason.h"
#include "algorithmmodel.h"
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
        bool setBackground();

    protected:
        void changeEvent(QEvent* e);

    private:
        Ui::MainWindow* _ui;
        QGraphicsScene* _scene;
        void resizeEvent(QResizeEvent* event);
        AlgorithmModel _beeAlgModel;
        int _initialPop;
        int _genCap;
        int _sites;
        int _eliteSites;
        QSize _fieldDims;
        void draw();
        void generateContourMap(double**);
        int _graduation;
        int _upperBound;
        int _lowerBound;
        const static int _GRADES = 10;
        const static int _SHEKEL_DIMENSION = 10;
        QRgb getColor(double);
        void setGraduation(double**);
        bool eventFilter(QObject* , QEvent* );
        SimulateSeason season;

    private slots:
        void on_fieldHeight_valueChanged(int);
        void on_fieldWidth_valueChanged(int);
        void on_initialPop_valueChanged(int);
        void on_genCap_valueChanged(int);
        void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
