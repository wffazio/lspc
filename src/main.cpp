#include "inc/mainwindow.h"
#include "inc/mydb.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    MyDb playlistDb;
    QApplication app(argc, argv);
    if (!playlistDb.connect())
    {
        return EXIT_FAILURE;
    }
    MainWindow w(playlistDb);
    w.show();

    return app.exec();
}
