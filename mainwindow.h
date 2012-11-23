#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "algorithmmodel.h"
#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool setBackground();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    void resizeEvent(QResizeEvent* event);
    AlgorithmModel beeAlgModel;
    int initialPop;
    int genCap;
    int sites;
    int eliteSites;
    QSize fieldDims;
    void draw();

private slots:
    void on_initialPop_valueChanged(int );
    void on_genCap_valueChanged(int );
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
