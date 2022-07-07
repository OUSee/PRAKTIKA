#ifndef POINTSTORAGE_H
#define POINTSTORAGE_H

#include <vector>
#include <QPoint>

class QPointStorage
{
public:
    QPointStorage();
    QPoint* searchPointsInRadius(int x, int y, int radius);
    void associate(QPoint &point);
    void erase(QPoint &point);
    void clear();
    size_t size();
    QPoint& operator[](size_t index);
protected:
    std::vector<QPoint*> points;
};

#endif // POINTSTORAGE_H
