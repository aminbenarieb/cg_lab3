#include "qpaintwidget.h"
#include "congif.h"

#include <QDebug>
#include <QPainter>

QPaintWidget::QPaintWidget(QWidget * parent) : QWidget(parent)
{
  // Initialization
    method = -1;
}

void QPaintWidget::paintEvent(QPaintEvent *) {


    QPainter ppainter(this);
    if (spectr)
    {
        drawSpectr(&ppainter, p1, lineCount, color);
    }
    else
    {
        switch (method) {
        case 0:
            drawDDALine(&ppainter, p1, p2, color);
            break;
        case 1:
            drawBrezenhamIntLine(&ppainter, p1, p2, color);
            break;
        case 2:
            drawBrezenhamFloatLine(&ppainter, p1, p2, color);
            break;
        case 3:
            drawBrezenhamSmoothLine(&ppainter, p1, p2, color);
            break;
        case 4:
            drawQtLine(&ppainter, p1, p2, color);
            break;
        default:
            break;
        }
    }

}


// Algorithms with drawing

void QPaintWidget::drawDDALine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){

    // Input data
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    // Intermediate calculation
    double x = x1, y = y1;
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);
    int l = dx > dy ? dx : dy;
    dx /= l;
    dy /= l;

    // Output drawing
    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    for (int i = 0; i < l; i++)
    {
       ppainter->drawPoint(round(x), round(y));
       x += dx;
       y += dy;
    }
     ppainter->restore();

}
void QPaintWidget::drawBrezenhamIntLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){}
void QPaintWidget::drawBrezenhamFloatLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){}
void QPaintWidget::drawBrezenhamSmoothLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){}
void QPaintWidget::drawQtLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){

    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    ppainter->drawLine(p1,p2);
    ppainter->restore();

}
void QPaintWidget::drawSpectr(QPainter *ppainter, QPoint p1,  int lineCount, QColor color){}
