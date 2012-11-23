#include "bee.h"
#include <vector>
#include <QSize>

using namespace std;

#ifndef ALGORITHMMODEL_H
#define ALGORITHMMODEL_H

class AlgorithmModel
{
public:
    AlgorithmModel();
    void genesis(const int population, const QSize fieldDims);
    const vector<Bee > getBees();

private:
    vector<Bee > bees;

};

#endif // ALGORITHMMODEL_H
