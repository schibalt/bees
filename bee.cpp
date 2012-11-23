#include "bee.h"

Bee::Bee()
{
}

void Bee::setPoint(QPoint newLocation)
{
    point = newLocation;
}

const QPoint Bee::getPoint()
{
    return  point;
}
