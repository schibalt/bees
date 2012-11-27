#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#ifndef SIMULATESEASON_H
#define SIMULATESEASON_H

class SimulateSeason : public QThread
{
    private:
        QMutex sync;
        QWaitCondition pauseCond;
        bool pause;
        int _seasonLength;
        bool _step;

    public:
        SimulateSeason();
        //SimulateSeason(...): pause(false) {}

        void resumeThread();
void setSeasonLength(int);
        void pauseThread();

    protected:
        void run();
};

#endif // SIMULATESEASON_H
