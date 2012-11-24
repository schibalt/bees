#include "bee.h"
#include <vector>
#include <QSize>
#include <time.h>
#include <QFuture>
#include <iostream>
#include "hive.h"

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

private:
    vector<Bee > bees;
    Hive hive;
};

#endif // ALGORITHMMODEL_H
