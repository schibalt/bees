/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Nov 23 00:06:50 2012
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QSpinBox *genCap;
    QLabel *label_2;
    QSpinBox *eliteSites;
    QLabel *label_3;
    QSpinBox *sites;
    QLabel *label;
    QSpinBox *initialPop;
    QLabel *label_5;
    QSpinBox *fieldWidth;
    QLabel *label_6;
    QSpinBox *fieldHeight;
    QPushButton *pushButton;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(134, 0));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        genCap = new QSpinBox(groupBox);
        genCap->setObjectName(QString::fromUtf8("genCap"));
        genCap->setMinimum(1);
        genCap->setMaximum(999);
        genCap->setValue(150);

        verticalLayout->addWidget(genCap);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(16777215, 26));

        verticalLayout->addWidget(label_2);

        eliteSites = new QSpinBox(groupBox);
        eliteSites->setObjectName(QString::fromUtf8("eliteSites"));
        eliteSites->setMinimum(1);
        eliteSites->setValue(1);

        verticalLayout->addWidget(eliteSites);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        sites = new QSpinBox(groupBox);
        sites->setObjectName(QString::fromUtf8("sites"));
        sites->setMinimum(1);
        sites->setValue(3);

        verticalLayout->addWidget(sites);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(88, 16777215));

        verticalLayout->addWidget(label);

        initialPop = new QSpinBox(groupBox);
        initialPop->setObjectName(QString::fromUtf8("initialPop"));
        initialPop->setMaximumSize(QSize(50, 16777215));
        initialPop->setMinimum(1);
        initialPop->setMaximum(50000);
        initialPop->setValue(50);

        verticalLayout->addWidget(initialPop);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(88, 16777215));

        verticalLayout->addWidget(label_5);

        fieldWidth = new QSpinBox(groupBox);
        fieldWidth->setObjectName(QString::fromUtf8("fieldWidth"));
        fieldWidth->setMaximumSize(QSize(50, 16777215));
        fieldWidth->setMinimum(10);
        fieldWidth->setMaximum(1000);
        fieldWidth->setValue(400);

        verticalLayout->addWidget(fieldWidth);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(88, 16777215));

        verticalLayout->addWidget(label_6);

        fieldHeight = new QSpinBox(groupBox);
        fieldHeight->setObjectName(QString::fromUtf8("fieldHeight"));
        fieldHeight->setMaximumSize(QSize(50, 16777215));
        fieldHeight->setMinimum(10);
        fieldHeight->setMaximum(1000);
        fieldHeight->setValue(300);

        verticalLayout->addWidget(fieldHeight);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label_4->setText(QApplication::translate("MainWindow", "Gen cap", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Elite Sites", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Sites", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Initial population", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Field width", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Field height", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Genesis", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
