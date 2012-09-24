#include <QtGui/QApplication>
#include <QTime>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    MainWindow w;
    w.show();
    return a.exec();
}
