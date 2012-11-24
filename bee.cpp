#include "bee.h"


//this condtructor is used for insting a bee
//at a random point on the field
Bee::Bee(const QSize fieldDims)
{
    setPointFieldRandom( QPoint(0, 0), fieldDims);
}

//this constructor is used for insting a bee
//at a random point in the neighborhood of
//some flowers
Bee::Bee(QPoint point, const QSize fieldDims)
{
    setPointFieldRandom(point, fieldDims);
}

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

void Bee::setPointFieldRandom(const QPoint point, const QSize fieldDims)
{
    unsigned int xPos = rand() % fieldDims.width();
    unsigned int yPos = rand() % fieldDims.height();
    setPoint(QPoint(xPos, yPos));
}
