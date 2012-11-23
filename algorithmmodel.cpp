
#include <QFuture>
#include "algorithmmodel.h"
#include <iostream>

AlgorithmModel::AlgorithmModel()
{
}

void AlgorithmModel::genesis(const int population, const QSize fieldDims)
{
    for(int i = 0; i < population; i++)
    {
        Bee newBee;
cout << "making bee " << i << endl;

        bees.push_back(newBee);
    }
}
