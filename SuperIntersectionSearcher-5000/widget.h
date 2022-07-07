#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QPointstorage.h"
#include "qlinestorage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

signals:
  void distanceChanged(float newDistance);

protected:
    QPointStorage pointStorage;
    QLineStorage lineStorage;
    void paintEvent(QPaintEvent *event);
    bool myDrawLine(QPainter *painter, QPoint &p1, QPoint &p2, QPen &linePen);
    void drawMovingVertex(QPainter *painter, QPen &pointPen, int radius);
    void drawMovingEdges(QPainter *painter, QPen &linePen);
    void drawConnectingEdge(QPainter *painter, QPen &linePen);

    float distance;
    QPoint mousePositon;
    std::vector<QPoint*> newPoints;
    QPoint apex;
    QPoint base1;
    QPoint base2;
    QPoint base3;
    QPoint base4;
    QPoint *movingPoint;
    QPoint *connectingPoint;
    QPoint selectedEdgePointStart;
    QPoint selectedEdgePointEnd;
    QPoint perpendicularPoint;
    std::pair<QPoint*, QPoint*>* selectedEdge;
    bool pointIsMoving;
    bool pointIsConnecting;
    bool edgeIsSelected;
    bool drawPerpendicularPoint;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};
#endif // WIDGET_H
