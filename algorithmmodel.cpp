#include <time.h>
#include <QFuture>
#include "algorithmmodel.h"
#include <iostream>

AlgorithmModel::AlgorithmModel()
{
}

vector<Bee > AlgorithmModel::genesis(const int population, const QSize fieldDims, bool add)
{
    if(!add)
        bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int i = 0; i < population; i++)
    {
        unsigned int xPos = rand() % fieldDims.width();
        unsigned int yPos = rand() % fieldDims.height();

        Bee newBee;
        newBee.setPoint(QPoint(xPos, yPos));
        //cout << "making bee " << i << endl;

        bees.push_back(newBee);
    }

    return bees;
}

void AlgorithmModel::setBees(vector<Bee > bees)
{
    this->bees = bees;
}

const vector<Bee > AlgorithmModel::getBees()
{
    cout << "returning bee vector size " << bees.size() << endl;
    return  bees;
}
