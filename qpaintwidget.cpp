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

    // если отрезок вырожденный, то отрисовываем только одну точку
    if (x2 == x1 && y2 == y1)
    {
        //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
           ppainter->drawPoint(x1, y1);
    }
    else
    {
        double xt=(double)x1, yt=(double)y1; //текущие координаты
        double dx=x2-x1, dy=y2-y1; //приращение координат
        int l= kmax( fabs(dx), fabs(dy)); //количество "итераций"
        dx /= l; dy /= l;
        for (int i=1; i<=l+1; i++)
        {
            //обрабатываем ТОЛЬКО пиксели внутри видимой обласи
            if (!((xt >= this->width()) || (xt < 0) || (yt >= this->height()) || (yt < 0)))
                ppainter->drawPoint((int)(xt+0.5), (int)(yt+0.5));
            xt += dx; yt += dy;
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


    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    if (x2 == x1 && y2 == y1)
    {
        //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    else
    {
        int dx=x2-x1, dy=y2-y1; //приращение координат
        int sx = ksign(dx), sy = ksign(dy); //шаг по X и по Y
        dx = abs(dx); dy = abs(dy); //абсолютируем приращения

        bool swap; //флаг обмена
        if (dy <= dx)
            swap = false;
        else
        {
            swap = true;
            int t = dx;
            dx = dy;
            dy = t;
        }

        int _E = 2*dy - dx; //ошибка
        int xt=x1, yt=y1; //текущие координаты

        //в цикле анализируем ошибку
        for (int i=1; i<=dx+1; i++)
        {
            //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
            {
                ppainter->drawPoint(xt, yt);
            }

            if (_E>=0)
            {
                if (swap)
                    xt += sx;
                else
                {
                    yt += sy;
                }
                _E = _E - 2*dx;
            }
            if (_E<0)
            {
                if (swap)
                    yt += sy;
                else
                {
                    xt += sx;
                }
            }
            _E = _E + 2*dy;
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


    ppainter->save();
    ppainter->setPen(QPen(color, kDrawPenWidth));
    if (x2 == x1 && y2 == y1)
    {
        //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    else
    {
        int dx=x2-x1, dy=y2-y1; //приращение координат
        int sx = ksign(dx), sy = ksign(dy); //шаг по X и по Y
        dx = abs(dx); dy = abs(dy); //абсолютируем приращения
        double m = (double)dy/dx; //тангенс угла наклона //(дабл) ОБЯЗАТЕЛЕН!! иначе он расценит (инт)ду/(инт)дх =0 при ду<дх!!

        bool swap; //флаг обмена
        if (m <= 1)
            swap = false;
        else
        {
            swap = true;
            int t = dx;
            dx = dy;
            dy = t;
            m = 1/m;
        }

        double e = m-0.5; //ошибка
        int xt=x1, yt=y1; //текущие координаты

        //в цикле анализируем ошибку
        for (int i=1; i<=dx+1; i++)
        {
            //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
                ppainter->drawPoint(xt, yt);

            if (e>=0)
            {
                if (swap)
                    xt += sx;
                else
                {
                    yt += sy;
                }
                e = e-1;
            }
            if (e<0)
            {
                if (swap)
                    yt += sy;
                else
                {
                    xt += sx;
                }
            }
            e = e+m;

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


    ppainter->save();
    if (x2 == x1 && y2 == y1)
    {
        //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
        if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
             ppainter->drawPoint(x1, y1);
    }
    else
    {
        int dx=x2-x1, dy=y2-y1; //приращение координат
        int sx = ksign(dx), sy = ksign(dy); //шаг по X и по Y
        dx = abs(dx); dy = abs(dy); //абсолютируем приращения

        double m = (double)dy/dx; //тангенс угла наклона
        bool swap; //флаг обмена
        if (m <= 1)
            swap = false;
        else
        {
            swap = true;
            int t = dx;
            dx = dy;
            dy = t;
            m = 1/m;
        }
#define I 256 //количество уровней интенсивности
        double e = I/2; //ошибка
        int xt=x1, yt=y1; //текущие координаты
        m *= I; //корректируем тангенс
        double W = I - m; //коэффициент

        //в цикле анализируем ошибку
        for (int i=1; i<=dx; i++)
        {
            int r,g,b; //ргб-составляющие цвета для закраски
            double C = e/I;
            //if (C>0.5) C = 0.5; //корректируем "прозрачность", чтобы перекрашиваться не в абсолютный ноль, а просто в бледный цвет
            r = color.red() + C*(255 - color.red());
            g = color.green() + C*(255 - color.green());
            b = color.blue() + C*(255 - color.blue());
            QColor drawcolor = QColor( 255,r,g,b );
            ppainter->setPen(QPen(drawcolor, kDrawPenWidth));
            //обрабатываем ТОЛЬКО пиксели внутри массива битмапа
            if (!((x1 >= this->width()) || (x1 < 0) || (y1 >= this->height()) || (y1 < 0)))
            {
                ppainter->drawPoint(xt, yt);
            }

            if (e <= W)
            {
                if (swap)
                    yt += sy;
                else
                {
                    xt += sx;
                }
                e = e+m;
            }
            else
            {
                xt += sx;
                yt += sy;
                e = e-W;
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
