#include "qlinestorage.h"
#include "cmath"

QLineStorage::QLineStorage()
{

}
std::vector<QPoint*> QLineStorage::searchConnectedPoints(QPoint &point)
{
    std::vector<QPoint*> connectedPoints;
    for (auto it : edges)
    {
        if (it.first == &point)
            connectedPoints.push_back(it.second);
        else if (it.second == &point)
            connectedPoints.push_back(it.first);
    }
    return connectedPoints;
}

std::pair<QPoint *, QPoint *>* QLineStorage::searchLinesInRadius(QPoint point, float radius)
{
    for (auto &it : edges)
    {
        if (getDistance(point, &it)<radius)
            return &it;
    }
    return nullptr;
}

std::pair<QPoint *, QPoint *>* QLineStorage::findEdge(QPoint cursor)
{
    for (auto &it : edges)
    {
        if ((cursor.x()-it.first->x())*(it.second->y()-it.first->y()) -
            (cursor.y()-it.first->y())*(it.second->x()-it.first->x()) == 0)
            return &it;
    }
    return nullptr;
}

std::vector<QPoint> QLineStorage::getIntersections()
{
    std::vector<QPoint> intersectionPoints;
    QPoint intersectionPoint;
    for (auto &edge1 : edges)
    {
        for (auto &edge2 : edges)
        {
            if (this->getIntersection(&edge1, &edge2,intersectionPoint) == true)
                intersectionPoints.push_back(intersectionPoint);
        }
    }
    return intersectionPoints;
}

std::vector<QPoint> QLineStorage::getIntersections(QPoint start, QPoint end)
{

}

bool QLineStorage::getIntersection(std::pair<QPoint *, QPoint *> *firstEdge, std::pair<QPoint *, QPoint *> *secondEdge, QPoint& intersectionPoint)
{
    float x11 = firstEdge->first->x();
    float y11 = firstEdge->first->y();
    float x12 = firstEdge->second->x();
    float y12 = firstEdge->second->y();

    float x21 = secondEdge->first->x();
    float y21 = secondEdge->first->y();
    float x22 = secondEdge->second->x();
    float y22 = secondEdge->second->y();

    //проверка на совпадение вершин

    if ((x11 == x22 && y11 == y22)||(x11 == x21 && y11 == y21)||(x12 == x22 && y12 == y22)||(x12 == x21 && y12 == y21))
      return false;

    //проверка на пересечение за длинами отрезков и параллельность
    float dx1 = x12-x11, dy1 = y12-y11; // Длина проекций первой линии на ось x и y
    float dx2 = x22-x21, dy2 = y22-y21; // Длина проекций второй линии на ось x и y
    float dxx = x11-x21, dyy = y11-y21;
    float div, mul;


    if ((div = (float)((double)dy2*dx1-(double)dx2*dy1)) == 0)
      return false; // Линии параллельны
    if (div > 0) {
      if ((mul = (float)((double)dx1*dyy-(double)dy1*dxx)) < 0 || mul > div)
        return false; // Первый отрезок пересекается за своими границами
      if ((mul = (float)((double)dx2*dyy-(double)dy2*dxx)) < 0 || mul > div)
         return false; // Второй отрезок пересекается за своими границами
    }

    if ((mul = -(float)((double)dx1*dyy-(double)dy1*dxx)) < 0 || mul > -div)
      return false; // Первый отрезок пересекается за своими границами
    if ((mul = -(float)((double)dx2*dyy-(double)dy2*dxx)) < 0 || mul > -div)
      return false; // Второй отрезок пересекается за своими границами

    // получаем уравнения прямых вида Ax+By+C = 0
    float A1 = y11-y12;
    float B1 = x12-x11;
    float C1 = y11*(x11-x12)+x11*(y12-y11);

    float A2 = y21-y22;
    float B2 = x22-x21;
    float C2 = y21*(x21-x22)+x21*(y22-y21);

    //считаем определитель {{A1,B1},{A2,B2}}
    float D = A1*B2-A2*B1;
    if (D == 0) // нет точки пересечения
        return false;
    else // иначе получаем точки пересечения по формуле Крамера (x = Dx/D, y = Dy/D)
    {
        intersectionPoint.setX(((-1)*C1*B2-(-1)*C2*B1)/D);
        intersectionPoint.setY(((-1)*C2*A1-(-1)*C1*A2)/D);
        return true;
    }
}

float QLineStorage::getDistance(QPoint from, std::pair<QPoint *, QPoint *>* edge)
{
    const float x1 = edge->first->x();
    const float y1 = edge->first->y();
    const float x2 = edge->second->x();
    const float y2 = edge->second->y();
    const float x = from.x();
    const float y = from.y();
    return (abs((y2-y1)*x-(x2-x1)*y+x2*y1-y2*x1))/(sqrt(pow((y2-y1),2)+pow((x2-x1),2)));
}

QPoint QLineStorage::getPerpendicular(QPoint from, std::pair<QPoint *, QPoint *> *edge)
{
    const float x1 = edge->first->x();
    const float y1 = edge->first->y();
    const float x2 = edge->second->x();
    const float y2 = edge->second->y();
    const float x = from.x();
    const float y = from.y();

    float k = ((y2-y1) * (x-x1) - (x2-x1) * (y-y1)) / (pow((y2-y1),2) + pow((x2-x1),2));
    QPoint perpendicularPoint;
    perpendicularPoint.setX(x - k * (y2-y1));
    perpendicularPoint.setY(y + k * (x2-x1));
    return perpendicularPoint;
}
void QLineStorage::connect(QPoint &point, QPoint &to)
{
    edges.push_back(std::make_pair(&point, &to));
}
void QLineStorage::disconnect(QPoint &point, QPoint &to)
{
    for (auto edge = edges.begin(); edge != edges.end(); ++edge)
    {

        if ((edge->first == &point && edge->second == &to) ||
            (edge->second == &point && edge->first == &to))
        {
            edges.erase(edge);
            break;
        }

    }
}

void QLineStorage::disconnect(std::pair<QPoint *, QPoint *> *edge)
{
    this->disconnect(*edge->first, *edge->second);
}
void QLineStorage::clear()
{
    edges.clear();
}

size_t QLineStorage::size()
{
    return edges.size();
}
std::pair<QPoint *, QPoint *> &QLineStorage::operator[](size_t index)
{
    return edges[index];
}
