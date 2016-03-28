#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QApplication>
#include <QDebug>

#include "ui_mainwindow.h"
#include "qpaintwidget.h"
#include "congif.h"

class MainWindow : public  QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QMainWindow *parent = 0);
    ~MainWindow();

    MainWindow(QMainWindow* pwgt = 0) : QMainWindow(pwgt) {


        //*****Setting Up******
        colours << QColor(5,5,5) << QColor(0,0,144) << QColor(144,0,0) << QColor(144,64,144) << QColor(0,144,0);
        setupUi(this);
        this->setWindowTitle(kTextTitle);
        //*******************************

        //***** QPaintWidget Settings ******
        wgt->setMouseTracking(true);
        wgt->installEventFilter(this);
        wgt->setAutoFillBackground(true);
        QPalette Pal = palette();
        Pal.setColor(QPalette::Background, Qt::white);
        wgt->setPalette(Pal);
        //*******************************

        //***** Buttons Settings ******
        btnClear->setText(kBtnTextDrawClear);
        btnDrawLine->setText(kBtnTextDrawLine);
        btnDrawSpectr->setText(kBtnTextDrawSpectr);
        //*******************************


        //***** Radio Buttons ******
        radioBtnDDA->setText(kRadioTextDDA);
        radioBtnIntBr->setText(kRadioTextIntBr);
        radioBtnFloatBr->setText(kRadioTextFloatBr);
        radioBtnSmtBr->setText(kRadioTextSmtBr);
        radioBtnQt->setText(kRadioTextQt);
        radioArray << radioBtnDDA << radioBtnIntBr << radioBtnFloatBr << radioBtnSmtBr << radioBtnQt;
        for (int i = 0; i < radioArray.count(); i++)
        {
            QPalette* palette = new QPalette();
            palette->setColor(QPalette::Foreground,colours[i]);
            radioArray[i]->setPalette(*palette);
        }
        //*******************************

        //***** Labels Settings ******
        labelAlgorithm->setText(kLabelAlorithm);
        labelLine->setText(kLabelLine);
        labelSpectr->setText(kLabelSpectr);
        //*******************************

        //***** LineEdit Settings ******
        lineEditSpectrX->setPlaceholderText(kPlaceHolderSpectrX);
        lineEditSpectrY->setPlaceholderText(kPlaceHolderSpectrY);
        lineEditLineCount->setPlaceholderText(kPlaceHolderSpectrLineCount);

        lineEditX1->setPlaceholderText(kPlaceHolderLineX1);
        lineEditY1->setPlaceholderText(kPlaceHolderLineY1);
        lineEditX2->setPlaceholderText(kPlaceHolderLineX2);
        lineEditY2->setPlaceholderText(kPlaceHolderLineY2);
        //*******************************

        QObject::connect(btnClear, SIGNAL(clicked()), this, SLOT(actionClearScreen()) );
        QObject::connect(btnDrawSpectr, SIGNAL(clicked()), this, SLOT(actionDrawSpectr()) );
        QObject::connect(btnDrawLine, SIGNAL(clicked()), this, SLOT(actionDrawLine()) );
        //*******************************

        QMenuBar mnuBar;

        QMenu* pmnu = new QMenu("Меню");
        pmnu->addSeparator();
        pmnu->addAction("&Нарисовать линию", this, SLOT(actionDrawLine()));
        pmnu->addAction("&Нарисовать спектр", this, SLOT(actionDrawSpectr()));
        pmnu->addAction("&Очистить", this, SLOT(actionQuit()));
        pmnu->addAction("&Выйти", this, SLOT(actionQuit()));

        mnuBar.addMenu(pmnu);
        mnuBar.show();

        this->setMenuBar(&mnuBar);
    }

public slots:
    void actionQuit(){
        QApplication::quit();
    }
    void actionClearScreen()
    {
        wgt->method = -1;
        wgt->update();
    }
    void actionDrawLine()
    {

        int x1, y1, x2, y2;
        bool flagX1, flagX2, flagY1, flagY2;

        x1 = lineEditX1->text().toInt(&flagX1);
        x2 = lineEditX2->text().toInt(&flagX2);
        y1 = lineEditY1->text().toInt(&flagY1);
        y2 = lineEditY2->text().toInt(&flagY2);

        if ( flagX1 && flagX2 && flagY1 && flagY2)
        {
            for (int i = 0; i < radioArray.count(); i++)
            {
                if (radioArray[i]->isChecked())
                {
                    wgt->method = i;
                    wgt->p1 = QPoint(x1, y1);
                    wgt->p2 = QPoint(x2, y2);
                    wgt->color = colours[i];
                    wgt->update();
                    break;
                }
            }
        }
        else
        {
            showMsg(kErrorMsgIncorrectData);
        }

    }
    void actionDrawSpectr()
    {

    }

private:
    bool eventFilter(QObject *, QEvent *);
    void showMsg(QString );
    QVector <QColor> colours;
    QVector <QRadioButton*> radioArray;

};

#endif // MAINWINDOW_H

