#include <QPoint>

#ifndef BEE_H
#define BEE_H

class Bee
{
public:
    Bee();
    void setPoint(QPoint newLocation);
    const QPoint getPoint();

private:
    QPoint point;
};

#endif // BEE_H
