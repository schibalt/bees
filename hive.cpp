#include "hive.h"

Hive::Hive()
{
}

Hive::Hive(const QSize fieldDims)
{
    setPointFieldRandom( QPoint(0, 0),  fieldDims);
}

void Hive::setPointFieldRandom(const QPoint point, const QSize fieldDims)
{
    unsigned int xPos = rand() % fieldDims.width();
    unsigned int yPos = rand() % fieldDims.height();
    setPoint(QPoint(xPos, yPos));
}

void Hive::setPoint(QPoint newLocation)
{
    point = newLocation;
}

QPoint Hive::getPoint()
{
    return point;
}
