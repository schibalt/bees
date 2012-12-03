/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Dec 2 18:56:46 2012
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
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *fieldHeight;
    QSpinBox *fieldWidth;
    QSpinBox *initialPop;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QSpinBox *genCap;
    QLabel *label_2;
    QSpinBox *eliteSites;
    QLabel *label_3;
    QLabel *label_7;
    QSpinBox *sites;
    QDoubleSpinBox *randCutBox;
    QCheckBox *stepBox;
    QPushButton *stepButton;
    QLabel *label_12;
    QLabel *label_13;
    QDoubleSpinBox *deltaLamba;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QDoubleSpinBox *deltaPhi;
    QPushButton *pushButton;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QLabel *label_8;
    QSpinBox *foxholes;
    QLabel *label_9;
    QSpinBox *maxima;
    QLabel *label_11;
    QSpinBox *power;
    QLabel *label_10;
    QSpinBox *bound;
    QCheckBox *deterBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 662);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 2, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 543));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(88, 16777215));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(88, 16777215));

        gridLayout_2->addWidget(label_5, 2, 0, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(88, 16777215));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        fieldHeight = new QSpinBox(groupBox_2);
        fieldHeight->setObjectName(QString::fromUtf8("fieldHeight"));
        fieldHeight->setMaximumSize(QSize(50, 16777215));
        fieldHeight->setMinimum(10);
        fieldHeight->setMaximum(1000);
        fieldHeight->setSingleStep(9);
        fieldHeight->setValue(500);

        gridLayout_2->addWidget(fieldHeight, 4, 1, 1, 1);

        fieldWidth = new QSpinBox(groupBox_2);
        fieldWidth->setObjectName(QString::fromUtf8("fieldWidth"));
        fieldWidth->setMaximumSize(QSize(50, 16777215));
        fieldWidth->setMinimum(10);
        fieldWidth->setMaximum(2000);
        fieldWidth->setSingleStep(10);
        fieldWidth->setValue(700);

        gridLayout_2->addWidget(fieldWidth, 2, 1, 1, 1);

        initialPop = new QSpinBox(groupBox_2);
        initialPop->setObjectName(QString::fromUtf8("initialPop"));
        initialPop->setMaximumSize(QSize(50, 16777215));
        initialPop->setMinimum(1);
        initialPop->setMaximum(50000);
        initialPop->setValue(50);

        gridLayout_2->addWidget(initialPop, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        genCap = new QSpinBox(groupBox_3);
        genCap->setObjectName(QString::fromUtf8("genCap"));
        genCap->setMinimum(1);
        genCap->setMaximum(999);
        genCap->setValue(150);

        gridLayout_4->addWidget(genCap, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(16777215, 26));

        gridLayout_4->addWidget(label_2, 1, 0, 1, 1);

        eliteSites = new QSpinBox(groupBox_3);
        eliteSites->setObjectName(QString::fromUtf8("eliteSites"));
        eliteSites->setMinimum(1);
        eliteSites->setValue(1);

        gridLayout_4->addWidget(eliteSites, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 2, 0, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 7, 0, 1, 1);

        sites = new QSpinBox(groupBox_3);
        sites->setObjectName(QString::fromUtf8("sites"));
        sites->setMinimum(1);
        sites->setValue(3);

        gridLayout_4->addWidget(sites, 2, 1, 1, 1);

        randCutBox = new QDoubleSpinBox(groupBox_3);
        randCutBox->setObjectName(QString::fromUtf8("randCutBox"));
        randCutBox->setMaximumSize(QSize(59, 16777215));
        randCutBox->setValue(10);

        gridLayout_4->addWidget(randCutBox, 7, 1, 1, 1);

        stepBox = new QCheckBox(groupBox_3);
        stepBox->setObjectName(QString::fromUtf8("stepBox"));
        stepBox->setChecked(true);

        gridLayout_4->addWidget(stepBox, 9, 0, 1, 1);

        stepButton = new QPushButton(groupBox_3);
        stepButton->setObjectName(QString::fromUtf8("stepButton"));
        stepButton->setEnabled(false);

        gridLayout_4->addWidget(stepButton, 9, 1, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_4->addWidget(label_12, 3, 0, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_4->addWidget(label_13, 4, 0, 1, 1);

        deltaLamba = new QDoubleSpinBox(groupBox_3);
        deltaLamba->setObjectName(QString::fromUtf8("deltaLamba"));
        deltaLamba->setValue(5);

        gridLayout_4->addWidget(deltaLamba, 3, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_4->addWidget(label_14, 8, 0, 1, 1);

        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_4->addWidget(label_15, 5, 0, 1, 1);

        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_4->addWidget(label_16, 6, 0, 1, 1);

        deltaPhi = new QDoubleSpinBox(groupBox_3);
        deltaPhi->setObjectName(QString::fromUtf8("deltaPhi"));
        deltaPhi->setValue(5);

        gridLayout_4->addWidget(deltaPhi, 5, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_3, 3, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_3->addWidget(pushButton, 2, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_5->addWidget(label_8, 0, 0, 1, 1);

        foxholes = new QSpinBox(groupBox_4);
        foxholes->setObjectName(QString::fromUtf8("foxholes"));
        foxholes->setValue(10);

        gridLayout_5->addWidget(foxholes, 0, 1, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_5->addWidget(label_9, 1, 0, 1, 1);

        maxima = new QSpinBox(groupBox_4);
        maxima->setObjectName(QString::fromUtf8("maxima"));
        maxima->setEnabled(true);
        maxima->setMinimum(1);
        maxima->setMaximum(1000);
        maxima->setValue(30);

        gridLayout_5->addWidget(maxima, 1, 1, 1, 1);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_5->addWidget(label_11, 3, 0, 1, 1);

        power = new QSpinBox(groupBox_4);
        power->setObjectName(QString::fromUtf8("power"));
        power->setValue(2);

        gridLayout_5->addWidget(power, 3, 1, 1, 1);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setEnabled(true);

        gridLayout_5->addWidget(label_10, 2, 0, 1, 1);

        bound = new QSpinBox(groupBox_4);
        bound->setObjectName(QString::fromUtf8("bound"));
        bound->setEnabled(false);
        bound->setValue(32);

        gridLayout_5->addWidget(bound, 2, 1, 1, 1);

        deterBox = new QCheckBox(groupBox_4);
        deterBox->setObjectName(QString::fromUtf8("deterBox"));

        gridLayout_5->addWidget(deterBox, 4, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_4, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
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
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Case params", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Initial population", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Field width", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Field height", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Run params", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Generations", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Elite Sites", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Priority sites", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Random search", 0, QApplication::UnicodeUTF8));
        stepBox->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
        stepButton->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "\316\224width", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "(% field long)", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "(% of colony)", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "\316\224height", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "(% field lat)", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Begin", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Shekel params", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Foxhole number", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Maxima/minima", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Power", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Bound", 0, QApplication::UnicodeUTF8));
        deterBox->setText(QApplication::translate("MainWindow", "Deterministic", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
