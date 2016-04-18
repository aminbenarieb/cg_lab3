#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QColorDialog>

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
        btnPerformance->setText(kBtnTextPerformance);
        //*******************************


        //***** Radio Buttons ******
        radioBtnDDA->setText(kRadioTextDDA);
        radioBtnIntBr->setText(kRadioTextIntBr);
        radioBtnFloatBr->setText(kRadioTextFloatBr);
        radioBtnSmtBr->setText(kRadioTextSmtBr);
        radioBtnQt->setText(kRadioTextQt);
        radioArray << radioBtnDDA << radioBtnIntBr << radioBtnFloatBr << radioBtnSmtBr << radioBtnQt;
        //*******************************

        //***** Labels Settings ******
        labelAlgorithm->setText(kLabelAlorithm);
        labelLine->setText(kLabelLine);
        labelSpectr->setText(kLabelSpectr);
        //*******************************

        //***** LineEdit Settings ******
        lineEditLineCount->setPlaceholderText(kPlaceHolderSpectrLineCount);

        lineEditX1->setPlaceholderText(kPlaceHolderLineX1);
        lineEditY1->setPlaceholderText(kPlaceHolderLineY1);
        lineEditX2->setPlaceholderText(kPlaceHolderLineX2);
        lineEditY2->setPlaceholderText(kPlaceHolderLineY2);
        //*******************************

        QObject::connect(btnClear, SIGNAL(clicked()), this, SLOT(actionClearScreen()) );
        QObject::connect(btnDrawSpectr, SIGNAL(clicked()), this, SLOT(actionDrawSpectr()) );
        QObject::connect(btnDrawLine, SIGNAL(clicked()), this, SLOT(actionDrawLine()) );
        QObject::connect(btnPerformance, SIGNAL(clicked()), this, SLOT(actionPerformance()) );
        QObject::connect(pushButtonLineColor, SIGNAL(clicked()), this, SLOT(actionLineColorChange()) );
        QObject::connect(pushButtonBackgrounColor, SIGNAL(clicked()), this, SLOT(actionBackgroundColorChange()) );
        //*******************************


        /* Default Color */

        pushButtonLineColor->setStyleSheet(QString("border-radius: 28px;"
                                                        "font-family: Arial;"
                                                        "color:white;"
                                                        "font-size: 20px;"
                                                        "padding: 10px 20px 10px 20px;"
                                                        "text-decoration: none;"
                                                    "background-color:black"));
       pushButtonBackgrounColor->setStyleSheet(QString("border-radius: 28px;"
                                                        "font-family: Arial;"
                                                        "font-size: 20px;"
                                                        "padding: 10px 20px 10px 20px;"
                                                        "text-decoration: none;"
                                                    "background-color:white"));

        /***/

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
        wgt->state = CleanState;
        wgt->lineInfoStack.clear();
        wgt->spectrInfoStack.clear();
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
                    wgt->state = LineState;
                    wgt->spectrInfoStack.clear();
                    wgt->lineInfoStack.append( (QLineInfo){QPoint(x1, y1),QPoint(x2, y2),lineColor,i} );
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
        int lineCount;
        bool flagLineCount;

        lineCount = lineEditLineCount->text().toInt(&flagLineCount);

        if ( flagLineCount)
        {
            for (int i = 0; i < radioArray.count(); i++)
            {
                if (radioArray[i]->isChecked())
                {
                    wgt->state = SpectrState;
                    wgt->lineInfoStack.clear();
                    wgt->spectrInfoStack.append( (QSpectrInfo){i, lineCount, lineColor } );
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
    void actionPerformance()
    {
        wgt->state = PerfomanceBarState;
        wgt->lineInfoStack.clear();
        wgt->spectrInfoStack.clear();
        wgt->update();
    }
    void actionLineColorChange()
    {

        QColor color;
        if (false)
            color = QColorDialog::getColor(Qt::green, this);
        else
            color = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);

        if (color.isValid())
        {
            lineColor = color;
            pushButtonLineColor->setStyleSheet(QString("border-radius: 28px;"
                                                       "font-family: Arial;"
                                                       "font-size: 20px;"
                                                       "padding: 10px 20px 10px 20px;"
                                                       "text-decoration: none;"
                                                       "background-color:%1").arg(color.name()));
        }

    }
    void actionBackgroundColorChange()
    {

        QColor color;
        if (false)
            color = QColorDialog::getColor(Qt::green, this);
        else
            color = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);

        if (color.isValid())
        {
            wgt->setPalette(QPalette(color));
            wgt->setAutoFillBackground(true);

            pushButtonBackgrounColor->setStyleSheet(QString("border-radius: 28px;"
                                                            "font-family: Arial;"
                                                            "font-size: 20px;"
                                                            "padding: 10px 20px 10px 20px;"
                                                            "text-decoration: none;"
                                                        "background-color:%1").arg(color.name()));
        }
    }

private:
    bool eventFilter(QObject *, QEvent *);
    void showMsg(QString );
    QColor lineColor;
    QVector <QRadioButton*> radioArray;

};

#endif // MAINWINDOW_H

