

#ifndef ALGORITHMMODEL_H
#define ALGORITHMMODEL_H

#include <vector>
#include <time.h>
#include <iostream>

#include <QObject>
#include <QSize>
#include <QFuture>
#include <QThread>

#include "hive.h"
#include "bee.h"
//#include "simulateseason.h"

using namespace std;

class AlgorithmModel
{
public:
    AlgorithmModel();
    const vector<Bee > getBees();
    void setBees(vector<Bee >);
    double** foxholes( );
    void computeField(double** );
    //void runSim(int);


private:
    vector<Bee > _bees;
    Hive _hive;
    double foxHelper(int* );
    const static int _shekelMaxima = 20;
    const static int _foxholeMatDimFactor = 5;
    const double** _field;
    int _seed;
    QSize _fieldDims;

};

#endif // ALGORITHMMODEL_H
