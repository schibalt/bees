#include <QPoint>
#include <QSize>
#include <time.h>
#include <cstdlib>

#ifndef BEE_H
#define BEE_H

class Bee
{
public:
    Bee();
    Bee(QSize);
    Bee(QPoint, QSize);
    void setPoint(QPoint );
    const QPoint getPoint();
    void setPointFieldRandom(const QPoint, const QSize );
    enum role { SCOUT, RECRUIT, ELITE };
    void setFitness(double);

private:
    QPoint point;
    double fitness;
};

#endif // BEE_H
