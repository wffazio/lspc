#include "inc/mainwindow.h"
#include "inc/mydb.hpp"
#include <QApplication>
#include <QDebug>


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
    MainWindow w(playlistDb);
    w.show();

    return app.exec();
}
