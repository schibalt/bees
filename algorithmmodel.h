#include "bee.h"
#include <vector>
#include <QSize>
#include <time.h>
#include <QFuture>
#include <iostream>
#include "hive.h"
#include "normal.hpp"

using namespace std;

#ifndef ALGORITHMMODEL_H
#define ALGORITHMMODEL_H

class AlgorithmModel
{
public:
    AlgorithmModel();
    vector<Bee > genesis(const int , const QSize, bool);
    const vector<Bee > getBees();
    void setBees(vector<Bee >);
    Hive getHive();
    void setHive(QSize);
    double** foxholes( );
    void computeField(double** );
    const double** getField();

private:
    vector<Bee > _bees;
    Hive _hive;
    double foxHelper(int* );
    const static int _SHEKEL_DIMENSION = 10;
    const double** _field;
    QSize _fieldDims;
};

#endif // ALGORITHMMODEL_H
