#include "mainwindow.h"
#include "congif.h"

#include <QGridLayout>
#include <QEvent>
#include <QStatusBar>
#include <QMessageBox>



MainWindow::~MainWindow()
{
    wgt->deleteLater();
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress )
    {
        qDebug()<<((QKeyEvent*)event)->key();
        return true;
    }
    else if (event->type() ==QMouseEvent::MouseMove)
    {
        QPointF pos = ((QMouseEvent*)event)->pos();
        this->setWindowTitle(  QString("%1 (%2,%3)").arg(kTextTitle, QString::number(pos.x()), QString::number(pos.y())) );
        return true;
    }
    else if (event->type() ==QEvent::Leave)
    {
        this->setWindowTitle(kTextTitle);

        return true;
    }

    return false;
}

void MainWindow::showMsg(QString msg)
{
    QMessageBox *msgBox = new QMessageBox(0);
    msgBox->setText("Сообщение");
    msgBox->setWindowModality(Qt::NonModal);
    msgBox->setInformativeText( msg );
    QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox->layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox->exec();
}
