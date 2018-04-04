#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "inc/mydb.hpp"


/*---------------------------------------------------------------------------*/
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
        this->tracksTable = "tracks";
        if (!db.tables().contains(this->tracksTable))
        {
            qDebug() << "First playlist DB creation";

            query.exec("create table tracks "
                       "(trackid integer PRIMARY KEY AUTOINCREMENT, "
                       "trackname varchar(64), "
                       "album varchar(64), "
                       "artist varchar(64), "
                       "preview varchar(256))");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('The Mission','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Lifetime','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Sad But True','Metallica (Black Album)','Metallica','')");
        }
        ret = true;
    }

    return ret;
}
