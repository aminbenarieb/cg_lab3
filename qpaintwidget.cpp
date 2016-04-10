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

    switch (state) {
    case PerfomanceBarState:
        drawPerfomanceBar(&ppainter);
        break;
    case SpectrState:
        drawSpectr(&ppainter, lineCount, color);
        break;
    case LineState:
        foreach (QLineInfo lineInfo, lineInfoStack)
        {
            method = lineInfo.method;
            drawLine(&ppainter, lineInfo.p1, lineInfo.p2, lineInfo.color);
        }
        break;
     default:
        break;
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

    // Output drawing
    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    // Degenerate case
    if (x2 == x1 && y2 == y1)
    {
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
           ppainter->drawPoint(x1, y1);
    }
    // General case
    else
    {
        // current coordinates
        double xt = (double) x1,
               yt = (double) y1,
        // x & y length
               dx = x2 - x1,
               dy = y2 - y1;
        // max length
        int l= kmax( fabs(dx), fabs(dy));
        // i-step increment
        dx /= l;
        dy /= l;
        for (int i = 1; i <= l+1; i++)
        {
            if (!((xt >= this->width()) || (xt < 0) || (yt >= this->height()) || (yt < 0)))
                // rounding value via (int) and half pixel
                ppainter->drawPoint((int)(xt+0.5), (int)(yt+0.5));

            // increment
            xt += dx;
            yt += dy;
        }
    }
    ppainter->restore();


}
void QPaintWidget::drawBrezenhamIntLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){

    // Input data
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    // Output drawing
    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    // Degenerate case
    if (x2 == x1 && y2 == y1)
    {
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    // General case
    else
    {
        // x & y i-step
        int dx = x2-x1,
            dy = y2-y1;
        // x & y step
        int sx = ksign(dx),
            sy = ksign(dy);
        // x & y length
        dx = abs(dx);
        dy = abs(dy);

        // exchange flag
        bool exchange;
        if (dy <= dx)
        {
            exchange = false;
        }
        else
        {
            exchange = true;
            kSwap(dx,dy);
        }


        int error = 2*dy - dx,        // initialization error adjusted by half a pixel
            xt = x1,                  // current coordinates
            yt = y1;

        for (int i = 1; i <= dx+1; i++)
        {
            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
            {
                ppainter->drawPoint(xt, yt);
            }

            if (error>=0)
            {
                if (exchange)
                {
                    xt += sx;
                }
                else
                {
                    yt += sy;
                }
                error = error - 2*dx;
            }
            if (error<0)
            {
                if (exchange)
                {
                    yt += sy;
                }
                else
                {
                    xt += sx;
                }
            }

            error = error + 2*dy;
        }
    }
    ppainter->restore();

}
void QPaintWidget::drawBrezenhamFloatLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){

    // Input data
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();


    // Output drawing
    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    // Degenerate case
    if (x2 == x1 && y2 == y1)
    {
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    // General case
    else
    {

        // x & y i-step
        int dx = x2-x1,
            dy = y2-y1;
        // x & y step
        int sx = ksign(dx),
            sy = ksign(dy);
        // x & y length
        dx = abs(dx);
        dy = abs(dy);

        // slope (tangent line)
        double scope = (double)dy/dx;

        bool exchange; //флаг обмена
        if (scope <= 1)
        {
            exchange = false;
        }
        else
        {
            exchange = true;
            kSwap(dx,dy);
            scope = 1/scope;
        }

        double error = scope-0.5;    // initialization error adjusted by half a pixel
        int xt=x1,                   // current coordinates
            yt=y1;

        for (int i = 1; i <= dx+1; i++)
        {
            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
            {
                ppainter->drawPoint(xt, yt);
            }

            if (error >= 0)
            {
                if (exchange)
                {
                    xt += sx;
                }
                else
                {
                    yt += sy;
                }
                error = error-1;
            }
            if (error < 0)
            {
                if (exchange)
                {
                    yt += sy;
                }
                else
                {
                    xt += sx;
                }
            }
            error = error + scope;
        }
    }
    ppainter->restore();

}
void QPaintWidget::drawBrezenhamSmoothLine(QPainter *ppainter, QPoint p1, QPoint p2, QColor color){

    // Input data
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();


    // Output drawing
    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    // Degenerate case
    if (x2 == x1 && y2 == y1)
    {
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    // General case
    else
    {
        // x & y i-step
        int dx = x2-x1,
            dy = y2-y1;
        // x & y step
        int sx = ksign(dx),
            sy = ksign(dy);
        // x & y length
        dx = abs(dx);
        dy = abs(dy);

        // slope (tangent line)
        double scope = (double)dy/dx;

        // exchange flag
        bool exchange;
        if (scope <= 1)
        {
            exchange = false;
        }
        else
        {
            exchange = true;
            kSwap(dx,dy);
            scope = 1/scope;
        }

        // intensity level count
        const double I = 256;

        double error = I/2;         // initialization error adjusted by half the intensity level count
        int xt=x1,                  // current coordinates
            yt = y1;

        // tangent line adjustment
        scope *= I;
        double W = I - scope; // weight coefficient

        for (int i = 1; i <= dx; i++)
        {
            int r,g,b;
            double C = error/I; // ?????
            r = color.red()   +  C * (255 - color.red()   );
            g = color.green() +  C * (255 - color.green() );
            b = color.blue()  +  C * (255 - color.blue()  );

            ppainter->setPen(QPen(QColor(r,g,b), kDrawPenWidth));

            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
            {
                ppainter->drawPoint(xt, yt);
            }

            if (error <= W)
            {
                if (exchange)
                    yt += sy;
                else
                {
                    xt += sx;
                }
                error = error+scope;
            }
            else
            {
                xt += sx;
                yt += sy;
                error = error-W;
            }
        }
    }
    ppainter->restore();

}
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
