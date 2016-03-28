#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>
#include <QVarLengthArray>



class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    QPaintWidget(QWidget * parent = 0);
    bool spectr;
    int method;
    int lineCount;
    QPoint p1;
    QPoint p2;
    QColor color;

protected:
    void paintEvent(QPaintEvent *);

private:
    void drawDDALine(QPainter *,QPoint, QPoint, QColor);
    void drawBrezenhamIntLine(QPainter *, QPoint, QPoint, QColor);
    void drawBrezenhamFloatLine(QPainter *, QPoint, QPoint, QColor);
    void drawBrezenhamSmoothLine(QPainter *, QPoint, QPoint, QColor);
    void drawQtLine(QPainter *, QPoint, QPoint, QColor);
    void drawSpectr(QPainter *, QPoint, int, QColor);



};

#endif // QPAINTWIDGET_H
