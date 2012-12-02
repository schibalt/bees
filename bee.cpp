#include "bee.h"


//this condtructor is used for insting a bee
//at a random point on the field
Bee::Bee(const QSize fieldDims)
{
    _role = SCOUT;
    setPointFieldRandom( QPoint(0, 0), fieldDims);
}

//this constructor is used for insting a bee
//at a random point in the neighborhood of
//some flowers
Bee::Bee(QPoint point, const QSize fieldDims)
{
    _role = SCOUT;
    setPointFieldRandom(point, fieldDims);
}

Bee::Bee()
{
_role = SCOUT;
}

void Bee::setPoint(QPoint newLocation)
{
    _point = newLocation;
}

const QPoint Bee::getPoint()
{
    return  _point;
}

void Bee::setPointFieldRandom(const QPoint point, const QSize fieldDims)
{
    unsigned int xPos = rand() % fieldDims.width();
    unsigned int yPos = rand() % fieldDims.height();
    setPoint(QPoint(xPos, yPos));
}

void Bee::setRole(int role)
{
    _role = role;
}

int Bee::getRole()
{
    return _role;
}

bool Bee::operator < (const Bee &thatBee) const
{
    const double thatBeesFitness =  thatBee.getFitness();
    return (getFitness() < thatBeesFitness);
}

 const double Bee::getFitness() const
{
    return _fitness;
}

void Bee::setFitness(double fitness)
{
    _fitness = fitness;
}
