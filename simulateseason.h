#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QObject>
#include <QDebug>

#ifndef SIMULATESEASON_H
#define SIMULATESEASON_H

class SimulateSeason : public QObject
{
    Q_OBJECT
    //public slots:

public:
    SimulateSeason();
    //SimulateSeason(...): pause(false) {}
    private:
        QMutex sync;
        QWaitCondition pauseCond;
        bool pause;
        int _seasonLength;
        bool _step;

        void resumeThread();
void setSeasonLength(int);
        void pauseThread();

    protected:
        void run();

};

#endif // SIMULATESEASON_H
