#ifndef SIMULATESEASON_H
#define SIMULATESEASON_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <iostream>

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
        void setFieldGenMembers(QThread&, int, int);
        const double** getFoxholes();
        //void setConnections(QThread &thread);

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
        int _shekelMaxima;
        int _foxholeNumber;
        double** _foxholes;
        double** _field;

        double foxHelper(int* , int);

        int _seasonLength;

    signals:
        void beesGenerated();
        void foxholesGenerated();
        void fieldGenerated();

    public slots:
        void foxholes();
        void computeField();
        void genesis();

    private slots:

};

#endif // SIMULATESEASON_H
