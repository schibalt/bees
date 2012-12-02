#ifndef SIMULATESEASON_H
#define SIMULATESEASON_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <iostream>
#include <algorithm>

#include "bee.h"
#include "hive.h"
#include "normal.hpp"

using namespace std;

class WorkerBee : public QObject
{
        Q_OBJECT

    public:
        explicit WorkerBee(QObject* parent = 0);

        //colony generation
        const vector<Bee >* getBees();
        Hive getHive();
        void setGenesisMembers(QThread&, int , QSize , bool);

        //field generation
        const double** getField();
        void setFieldGenMembers(QThread&, int, int, int, int);
        const double** getFoxholes();
        //void setConnections(QThread &thread);
        void setFitnessEvalMembers(QThread&, int, int);

    private:
        void disconnectEverything(QThread&);

        //colony generation
        int _population;
        vector<Bee > _bees;
        Hive _hive;
        QSize _fieldDims;
        bool _add;

        void setHive();

        //field generation
        int _maxima;
        int _foxholeParam;
        int _bound;
        int _power;
        const static int _DIMENSIONS = 2;
        double** _foxholes;
        double** _field;

        double foxHelper(int* );

        //fitness evaluation
        int _sites;
        int _eliteSites;

        int _seasonLength;

    signals:
        void beesGenerated();
        void foxholesGenerated();
        void fieldGenerated();
        void fitnessesEvaluated();

        void quitBeeGenThread();
        void quitFieldGenThread();
        void quitFitEvalThread();

    public slots:
        void foxholes();
        void computeField();
        void genesis();
        void evaluateFitnesses();
        void evaluateFitnesses(vector<Bee > );

    private slots:

};

#endif // SIMULATESEASON_H
