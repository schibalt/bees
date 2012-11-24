
#include "algorithmmodel.h"

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
        Bee newBee(fieldDims);
        //newBee.setPoint(QPoint(xPos, yPos));
        //cout << "making bee " << i << endl;

        bees.push_back(newBee);
    }

    return bees;
}

void AlgorithmModel::setHive(const QSize fieldDims)
{
    hive = Hive(fieldDims);
}

Hive AlgorithmModel::getHive()
{
    return hive;
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
