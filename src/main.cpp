#include "inc/appmainwindow.h"
#include "inc/mydb.hpp"
#include <QApplication>
#include <QDebug>
#include "inc/tabbedwindow.h"


/*---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    MyDb playlistDb;
    QApplication app(argc, argv);
    if (!playlistDb.connect())
    {
        qDebug() << "Db NOT connected";
        return EXIT_FAILURE;
    }
    qDebug() << "Db connected";
    AppMainWindow w(playlistDb);
    w.show();
    //AppWindowManager::TabbedMainWindow tw(playlistDb);
    //tw.show();

    return app.exec();
}
