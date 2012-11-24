#include <QPoint>
#include <QSize>
#include <cstdlib>

#ifndef HIVE_H
#define HIVE_H

class Hive
{
public:
    Hive();
    Hive(QSize);
    QPoint getPoint();

private:
    void setPointFieldRandom(const QPoint, const QSize );
    void setPoint(QPoint );
    QPoint point;
};

#endif // HIVE_H
