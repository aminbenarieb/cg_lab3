#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>
#include <QVarLengthArray>

struct QLineInfo
{
    QPoint p1;
    QPoint p2;
    QColor color;
    int method;
};

struct QSpectrInfo
{
    int method;
    int lineCount;
    QColor color;
};

enum QPaintState
{
    CleanState,
    PerfomanceBarState,
    SpectrState,
    LineState
};

class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    QPaintWidget(QWidget * parent = 0); 
    int method;
    QPaintState state;
    QVector <QLineInfo> lineInfoStack;
    QVector <QSpectrInfo> spectrInfoStack;


protected:
    void paintEvent(QPaintEvent *);

private:
    void drawLine(QPainter *,QPoint, QPoint, QColor);
    void drawDDALine(QPainter *,QPoint, QPoint, QColor);
    void drawBrezenhamIntLine(QPainter *, QPoint, QPoint, QColor);
    void drawBrezenhamFloatLine(QPainter *, QPoint, QPoint, QColor);
    void drawBrezenhamSmoothLine(QPainter *, QPoint, QPoint, QColor);
    void drawQtLine(QPainter *, QPoint, QPoint, QColor);
    void drawSpectr(QPainter *, int, QColor);
    void drawPerfomanceBar(QPainter *);



};

#endif // QPAINTWIDGET_H
