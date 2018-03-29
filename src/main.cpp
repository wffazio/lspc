#include "inc/mainwindow.h"
#include "inc/mydb.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MyDb playlistDb;

    playlistDb.connect();
    w.show();

    return a.exec();
}
