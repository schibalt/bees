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
    vector<Bee > genesis(const int , const QSize, bool);
    const vector<Bee > getBees();
    void setBees(vector<Bee >);

private:
    vector<Bee > bees;
};

#endif // ALGORITHMMODEL_H
