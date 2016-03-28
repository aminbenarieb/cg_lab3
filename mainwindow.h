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

        setupUi(this);

        this->setWindowTitle(kTextTitle);

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
    void actionSetUp()
    {
        updateScreen();
    }
    void actionQuit(){
        QApplication::quit();
    }
    void actionClearScreen()
    {

    }
    void actionDrawLine()
    {

    }
    void actionDrawSpectr()
    {

    }

private:
    bool eventFilter(QObject *, QEvent *);
    void updateScreen()
    {

        wgt->update();
    }
    void showMsg(QString );

protected:
    void showEvent(QShowEvent *e){
        QWidget::showEvent(e);
        QCoreApplication::processEvents();

        actionSetUp();
    }


};

#endif // MAINWINDOW_H

