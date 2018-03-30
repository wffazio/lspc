#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include "inc/mydb.hpp"

bool MyDb::connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("playlist.db");
    QSqlQuery query;
    bool ret = false;
    if (!db.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir playlist.db"),
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    else
    {
        this->activeTable = "tracks";

        if (!db.tables().contains(this->activeTable))
        {

            query.exec("create table tracks (trackid integer PRIMARY KEY AUTOINCREMENT, artist varchar(64), trackname varchar(64))");

            query.exec("insert into tracks (artist,trackname) values('Van Canto', 'The Mission')");
            query.exec("insert into tracks (artist,trackname) values('Van Canto', 'Lifetime')");
            query.exec("insert into tracks (artist,trackname) values('Metallica', 'Sad But True')");
        }


        ret = true;
    }

    return ret;
}
