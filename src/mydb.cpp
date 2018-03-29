#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include "inc/mydb.hpp"

bool MyDb::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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
        query.exec("create table tracks (trackid integer primary key autoincrement, "
                   "artist varchar(64), "
                   "trackname varchar(64))");

        query.exec("insert into tracks values('Van Canto', 'The Mission'");
        query.exec("insert into tracks values('Van Canto', 'Lifetime'");
        query.exec("insert into tracks values('Metallica', 'Sad But True'");

        ret = true;
    }

    return ret;
}
