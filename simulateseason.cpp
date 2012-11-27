#include "simulateseason.h"

SimulateSeason::SimulateSeason(/*int seasonLength*/)
{
    //_seasonLength = seasonLength;
    pause = 0;
}

//SimulateSeason::SimulateSeason(...): pause(false) {}

void SimulateSeason::resumeThread()
{
    sync.lock();
    pause = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void SimulateSeason::pauseThread()
{
    sync.lock();
    pause = true;
    sync.unlock();
}

void SimulateSeason::run()
{
    int dayCount = 0;

    while (dayCount < _seasonLength) // gues it's your loop
    {
        //qDebug() << "day " << dayCount << endl;
        sync.lock();
        if (pause)
            pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
        sync.unlock();

        // do your operation
        dayCount++;
    }
}

void SimulateSeason::setSeasonLength(int seasonLength)
{
    _seasonLength = seasonLength;
}
