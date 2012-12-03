#include <QPoint>
#include <QSize>
#include <time.h>
#include <cstdlib>

#ifndef BEE_H
#define BEE_H

struct Bee
{
public:
    Bee();
    Bee(QSize);
    Bee(QPoint, QSize);
    void setPoint(QPoint );
    const QPoint getPoint();
    void setFieldPtRandom(const QPoint, const QSize );
    void setFitness(double);
    void setRole(int );
    int getRole();
    const double getFitness() const;
    bool operator < (const Bee &) const;

    enum _roles { SCOUT, RECRUIT, PRIORITY, ELITE };

private:
    QPoint _point;
    double _fitness;
    int _role;
};

#endif // BEE_H
