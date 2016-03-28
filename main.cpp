#include <QApplication>

#include "mainwindow.h"
//#include "KeyFilter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.windowTitle() = kTextTitle;
    w.show();

    return app.exec();
}
