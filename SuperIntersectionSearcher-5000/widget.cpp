#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    apex.setX(200); apex.setY(50);
    base1.setX(100); base1.setY(150);
    base2.setX(50); base2.setY(250);
    base3.setX(300); base3.setY(250);
    base4.setX(350); base4.setY(150);

    pointStorage.associate(apex);
    pointStorage.associate(base1);
    pointStorage.associate(base2);
    pointStorage.associate(base3);
    pointStorage.associate(base4);

    lineStorage.connect(apex, base1);
    lineStorage.connect(apex, base2);
    lineStorage.connect(apex, base3);
    lineStorage.connect(apex, base4);

    lineStorage.connect(base1, base2);
    lineStorage.connect(base2, base3);
    lineStorage.connect(base3, base4);
    lineStorage.connect(base4, base1);

    distance = 0;

    pointIsMoving = false;
    pointIsConnecting = false;
    edgeIsSelected = false;
    drawPerpendicularPoint = false;
    movingPoint = nullptr;
    selectedEdge = nullptr;
}

Widget::~Widget()
{
    for (auto newPoint : newPoints)
        delete newPoint;
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter *painter = new QPainter(this);
    QPen myLinePen;

    QPen myPointPen(Qt::blue);
    myPointPen.setWidth(3);

    if (drawPerpendicularPoint)
    {
        myLinePen.setColor(Qt::gray);
        myLinePen.setWidth(1);
        myDrawLine(painter, *connectingPoint, perpendicularPoint, myLinePen);
        myDrawLine(painter, selectedEdgePointStart, perpendicularPoint, myLinePen);
        myDrawLine(painter, selectedEdgePointEnd, perpendicularPoint, myLinePen);
    }

    myLinePen.setColor(Qt::blue);
    myLinePen.setWidth(2);
    for (size_t i = 0; i < pointStorage.size(); i++)
    {
        std::vector<QPoint*> connectedPoints = lineStorage.searchConnectedPoints(pointStorage[i]);
        for (auto connectedPoint : connectedPoints)
            myDrawLine(painter, pointStorage[i], *connectedPoint, myLinePen);

        painter->setPen(myPointPen);
        painter->drawEllipse(pointStorage[i], 2, 2);
    }

    if (edgeIsSelected)
    {
        myLinePen.setColor(Qt::magenta);
        myLinePen.setWidth(2);
        myDrawLine(painter, selectedEdgePointStart, selectedEdgePointEnd, myLinePen);
    }

    myPointPen.setColor(Qt::green);
    myPointPen.setWidth(3);
    painter->setPen(myPointPen);
    std::vector<QPoint> intersectionPoints = lineStorage.getIntersections();
    for (auto intersectionPoint : intersectionPoints)
    {
        qDebug() << "123";
        painter->drawEllipse(intersectionPoint, 2, 2);
    }

    if (pointIsMoving)
    {
        myLinePen.setColor(Qt::yellow);
        myLinePen.setWidth(2);
        drawMovingEdges(painter, myLinePen);

        myPointPen.setColor(Qt::red);
        myPointPen.setWidth(3);
        drawMovingVertex(painter, myPointPen, 2);
    }

    if (pointIsConnecting)
    {
        myLinePen.setColor(Qt::green);
        myLinePen.setWidth(2);
        drawConnectingEdge(painter, myLinePen);
    }

    delete painter;
}

void Widget::drawMovingVertex(QPainter *painter, QPen &pen, int radius)
{
    painter->setPen(pen);
    painter->drawEllipse(*movingPoint, radius, radius);
}
void Widget::drawMovingEdges(QPainter *painter, QPen &linePen)
{
    std::vector<QPoint*> connectedPoints = lineStorage.searchConnectedPoints(*movingPoint);

    for (auto connectedPoint : connectedPoints)
        myDrawLine(painter, *movingPoint, *connectedPoint, linePen);
}
void Widget::drawConnectingEdge(QPainter *painter, QPen &linePen)
{
    myDrawLine(painter, *connectingPoint, mousePositon, linePen);
}
bool Widget::myDrawLine(QPainter *painter, QPoint &p1, QPoint &p2, QPen &linePen)
{
    painter->setPen(linePen);
    painter->drawLine(p1,p2);

    return true;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    drawPerpendicularPoint = false;
    if (event->button() == Qt::LeftButton)
    {
        int mouseX = event->x();
        int mouseY = event->y();
        movingPoint = pointStorage.searchPointsInRadius(mouseX, mouseY, 10);
        if (movingPoint != nullptr)
        {
            pointIsMoving = true;
        }
        else
        {
            pointIsMoving = false;
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        int mouseX = event->x();
        int mouseY = event->y();
        mousePositon.setX(mouseX);
        mousePositon.setY(mouseY);
        connectingPoint = pointStorage.searchPointsInRadius(mouseX, mouseY, 10);
        if (connectingPoint != nullptr)
        {
            pointIsConnecting = true;
        }
        else
        {
            pointIsConnecting = false;
        }
    }
    update();
}

void Widget::mouseMoveEvent (QMouseEvent *event)
{
    if (pointIsMoving)
    {
        *movingPoint = event->pos();
    }
    if (pointIsConnecting)
    {
        mousePositon = event->pos();
    }
    std::pair<QPoint *, QPoint *>* edge = lineStorage.searchLinesInRadius(event->pos(),10);
    if (edge != nullptr)
    {
        selectedEdgePointStart = *(edge->first);
        selectedEdgePointEnd = *(edge->second);
        edgeIsSelected = true;
        selectedEdge = edge;
    }
    else
        edgeIsSelected = false;

    update();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bool pointConnected = false;
    if (pointIsMoving)
    {
        *movingPoint = event->pos();
    }
    if (pointIsConnecting)
    {
        QPoint* connectedPoint = pointStorage.searchPointsInRadius(event->x(), event->y(), 10);
        if (connectedPoint != nullptr)
        {
            lineStorage.connect(*connectingPoint, *connectedPoint);
            pointConnected = true;
        }
    }
    if (pointIsConnecting && edgeIsSelected && !pointConnected)
    {
        emit distanceChanged(lineStorage.getDistance(*connectingPoint, selectedEdge));
        perpendicularPoint = lineStorage.getPerpendicular(*connectingPoint, selectedEdge);
        drawPerpendicularPoint = true;
    }
    pointIsMoving = false;
    pointIsConnecting = false;
    update();
}
void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    pointIsMoving = false;
    pointIsConnecting = false;
    QPoint* deletingPoint = pointStorage.searchPointsInRadius(event->x(), event->y(), 10);
    if (deletingPoint != nullptr)
    {
        std::vector<QPoint*> connectedPoints = lineStorage.searchConnectedPoints(*deletingPoint);
        for (auto connectedPoint : connectedPoints)
        {
            lineStorage.disconnect(*deletingPoint, *connectedPoint);
            lineStorage.disconnect(*connectedPoint, *deletingPoint);
        }
        pointStorage.erase(*deletingPoint);
    }
    else if (edgeIsSelected)
    {
        lineStorage.disconnect(selectedEdge);
        edgeIsSelected = false;
    }
    else
    {
        QPoint* newPoint = new QPoint(event->x(), event->y());
        newPoints.push_back(newPoint);
        pointStorage.associate(*newPoints.back());
    }
    update();
}
