
#include "algorithmmodel.h"
#include <normal.hpp>

AlgorithmModel::AlgorithmModel()
{
}

void AlgorithmModel::setBees(vector<Bee > bees)
{
    this->_bees = bees;
}

const vector<Bee > AlgorithmModel::getBees()
{
    cout << "returning bee vector size " << _bees.size() << endl;
    return  _bees;
}

/*
void AlgorithmModel::runSim(int seasonLength)
{

    SimulateSeason *season = new SimulateSeason();
    season->setSeasonLength(seasonLength);

    QThread *thread = new QThread();
    season->moveToThread(thread);

    QObject::connect( thread, SIGNAL(started()), season, SLOT(RunSim()) );
     QObject::connect( season, SIGNAL(stepComplete()), thread, SLOT(quit()) );
     //automatically delete thread and task object when work is done:
     QObject::connect( thread, SIGNAL(finished()), season, SLOT(deleteLater()) );
     QObject::connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );

     vector<Bee > sites;
     vector<Bee > elite;
     vector<Bee > random;

     for(int day = 0; day < seasonLength; day++)
     {
         //_bees = evaluteFitness(_bees);
         //sites = selectSites(_bees);
         //elite = selectElite(sites);

     }

    thread->start();
}

void AlgorithmModel::stepComplete(int)
{
}

*/
