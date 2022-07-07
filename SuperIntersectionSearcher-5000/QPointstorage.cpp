#include "QPointstorage.h"
#include <algorithm>

QPointStorage::QPointStorage()
{

}

QPoint* QPointStorage::searchPointsInRadius(int x, int y, int radius)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i]->x() <= x+radius && points[i]->x() >= x-radius
         && points[i]->y() <= y+radius && points[i]->y() >= y-radius)
        {
            return points[i];
        }
    }
    return nullptr;
}

void QPointStorage::associate(QPoint &point)
{
    points.push_back(&point);
}
void QPointStorage::erase(QPoint &point)
{
    auto pointToErase = std::find(points.begin(), points.end(), &point);
    if (pointToErase != points.end())
        points.erase(pointToErase);
}
void QPointStorage::clear()
{
    points.clear();
}
size_t QPointStorage::size()
{
    return points.size();
}
QPoint& QPointStorage::operator[](size_t index)
{
    return *points[index];
}
