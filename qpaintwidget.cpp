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
    if (performanceBar)
    {
        drawPerfomanceBar(&ppainter);
    }
    else if (spectr)
    {
        drawSpectr(&ppainter, lineCount, color);
    }
    else
    {
        drawLine(&ppainter, p1, p2, color);
    }

}


// Algorithms with drawing

void QPaintWidget::drawLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color)
{
    switch (method) {
    case 0:
        drawDDALine(ppainter, p1, p2, color);
        break;
    case 1:
        drawBrezenhamIntLine(ppainter, p1, p2, color);
        break;
    case 2:
        drawBrezenhamFloatLine(ppainter, p1, p2, color);
        break;
    case 3:
        drawBrezenhamSmoothLine(ppainter, p1, p2, color);
        break;
    case 4:
        drawQtLine(ppainter, p1, p2, color);
        break;
    default:
        break;
    }
}

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
void QPaintWidget::drawSpectr(QPainter *ppainter, int lineCount, QColor color){

    int x1 = this->width()/2;
    int y1 = this->height()/2;

    double angle = 360/lineCount;
    double length = kmin(this->width(),this->height());

    int x2, y2;
    for (double phi = 0; phi < 360; phi += angle)
    {
        x2 = round(x1 + length * cos( kDegreeToRadian(phi) ) );
        y2 = round(y1 + length * sin( kDegreeToRadian(phi) ) );
        drawLine(ppainter, QPoint(x1,y1),QPoint(x2,y2), color);
    }

}
void QPaintWidget::drawPerfomanceBar(QPainter *ppainter)
{
    QVector <int> times;
    for (int i = 0; i < 5; i++)
    {
        method = i;
        double time = 0;
        for (int i=0; i<100; i++)
        {
            double st = tick();
            drawLine(ppainter, QPoint(-100,-100), QPoint(0,0), Qt::transparent);
            double et = tick();
            time += et-st;
        }
        times.append( time / 100 );
    }

    double k = ((double)this->height()-100) / *std::max_element(times.begin(), times.end());
    int xStep = this->width() / (5*4);
    int x = xStep;
    QString algNames[] = {kRadioTextDDA, kRadioTextIntBr, kRadioTextFloatBr, kRadioTextSmtBr, kRadioTextQt};
    ppainter->save();
    ppainter->setBrush(Qt::blue);
    QFont font = ppainter->font();
    font.setPointSize(8);
    ppainter->setFont(font);
    for (int i = 0; i < 5; i++)
    {
        qDebug()<<times[i]<<" * "<<k<<" = "<<times[i]*k;
        ppainter->drawRect(x, this->height()-times[i]*k, xStep * 2, times[i] * k);
        ppainter->drawText(QPoint(x, this->height() - times[i]*k - 20), QString::number(times[i]) );
        ppainter->drawText(QRect(x, this->height() - times[i]*k - 50, 3*xStep, 40), algNames[i]);
        x += (xStep * 4);
    }
    ppainter->restore();
}
