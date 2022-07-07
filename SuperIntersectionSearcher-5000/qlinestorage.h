#ifndef QLINESTORAGE_H
#define QLINESTORAGE_H

#include <vector>
#include <QPoint>

class QLineStorage
{
public:
    QLineStorage();
    std::vector<QPoint*> searchConnectedPoints(QPoint &point);
    std::pair<QPoint*, QPoint*>* searchLinesInRadius(QPoint point, float radius);
    std::pair<QPoint*, QPoint*>* findEdge(QPoint cursor);
    std::vector<QPoint> getIntersections();
    std::vector<QPoint> getIntersections(QPoint start, QPoint end);
    bool getIntersection(std::pair<QPoint*, QPoint*>* firstEdge, std::pair<QPoint*, QPoint*>* secondEdge, QPoint& intersectionPoint);
    float getDistance(QPoint from, std::pair<QPoint*, QPoint*>* edge);
    QPoint getPerpendicular(QPoint from, std::pair<QPoint*, QPoint*>* edge);
    void connect(QPoint &point, QPoint &to);
    void disconnect(QPoint &point, QPoint &to);
    void disconnect(std::pair<QPoint*, QPoint*>* edge);
    void clear();
    size_t size();
    std::pair<QPoint*, QPoint*>& operator[](size_t index);
protected:
    std::vector<std::pair<QPoint*, QPoint*>> edges;
};

#endif // QLINESTORAGE_H
