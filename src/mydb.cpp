#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "inc/mydb.hpp"

#define CREATE_DUMMY_DB

/*---------------------------------------------------------------------------*/
MyDb::MyDb()
{

}


/*---------------------------------------------------------------------------*/
bool MyDb::openTracksTable()
{   
    this->playListDb_ = QSqlDatabase::addDatabase("QSQLITE","playlist");
    this->playListDb_.setDatabaseName(this->playlistDBName_);
    QSqlQuery query(this->playListDb_);
    bool ret = false;
    if (!this->playListDb_.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir ")+this->tracksTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    else
    {
        if (!this->playListDb_.tables().contains(this->tracksTableName_))
        {
            qDebug() << "First playlist DB creation";

            query.exec("create table " + this->tracksTableName_ +
                       "(trackid integer PRIMARY KEY AUTOINCREMENT, "
                       "trackname varchar(64), "
                       "album varchar(64), "
                       "artist varchar(64), "
                       "preview varchar(256))");
#ifdef CREATE_DUMMY_DB
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('The Mission','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Lifetime','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Sad But True','Metallica (Black Album)','Metallica','')");
#endif
        }
        this->playListDb_.close();
        ret = true;
    }

    return ret;
}


/*---------------------------------------------------------------------------*/
QString MyDb::tracksTableName() const
{
    return tracksTableName_;
}


/*---------------------------------------------------------------------------*/
bool MyDb::addTrack(const QVariantMap &insertTracks)
{
    bool insertSuccessfully;
    if (insertTracks.isEmpty())
    {
        qDebug()<<QStringLiteral("Fatal error on insert! "
                                 "The insertTracks is empty!");
        return false;
    }
    if (!this->playListDb_.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir ")+this->tracksTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return false;
    }

    QStringList strValues;
    QStringList fields = insertTracks.keys();
    QVariantList values = insertTracks.values();
    int totalFields = fields.size();
    for (int i = 0; i < totalFields; ++i)
        strValues.append("?");

    QString sqlQueryString = "insert into " + this->tracksTableName_ +
            " (" + QString(fields.join(",")) + ") values(" +
            QString(strValues.join(",")) + ")";
    QSqlQuery query("addQuery",this->playListDb_);
    query.prepare(sqlQueryString);

    int k = 0;
    foreach (const QVariant &value, values)
        query.bindValue(k++, value);
    insertSuccessfully =query.exec();
    if (insertSuccessfully)
    {
        emit MyDb::tracksInsertedSig(insertTracks);
    }
    this->playListDb_.close();
    return insertSuccessfully;

}


/*---------------------------------------------------------------------------*/
bool MyDb::createSearchTable()
{
    this->searchResultDb_ = QSqlDatabase::addDatabase("QSQLITE","searchresults");
    this->searchResultDb_.setDatabaseName(this->searchResultsDBName_);
    QSqlQuery query(this->playListDb_);
    bool ret = false;
    if (!this->searchResultDb_.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir ")+this->searchResultsTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    else
    {
        if (!this->searchResultDb_.tables().contains(this->searchResultsTableName_))
        {
            qDebug() << "First search DB creation";

            query.exec("create table " + this->searchResultsTableName_ +
                       " (trackid integer PRIMARY KEY AUTOINCREMENT, "
                       "trackname varchar(64), "
                       "album varchar(64), "
                       "artist varchar(64), "
                       "preview varchar(256))");
#ifdef CREATE_DUMMY_DB
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('The Mission2','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Lifetime2','Hero','Van Canto','')");
            query.exec("insert into tracks (trackname,album,artist,preview) "
                       "values('Sad But True2','Metallica (Black Album)','Metallica','')");
#endif
        }
        this->searchResultDb_.close();
        ret = true;
    }

    return ret;
}




/*---------------------------------------------------------------------------*/
bool MyDb::addSearchResults(const QVariantMap &insertTracks)
{
    bool insertSuccessfully;
    if (insertTracks.isEmpty())
    {
        qDebug()<<QStringLiteral("Fatal error on insert! "
                                 "The insertTracks is empty!");
        return false;
    }
    if (!this->searchResultDb_.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir") +this->searchResultsTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return false;
    }

    QStringList strValues;
    QStringList fields = insertTracks.keys();
    QVariantList values = insertTracks.values();
    int totalFields = fields.size();
    for (int i = 0; i < totalFields; ++i)
        strValues.append("?");

    QString sqlQueryString = "insert into " + this->searchResultsTableName_ +
            " (" + QString(fields.join(",")) + ") values(" +
            QString(strValues.join(",")) + ")";
    QSqlQuery query("addResultsQuery",this->searchResultDb_);
    query.prepare(sqlQueryString);

    int k = 0;
    foreach (const QVariant &value, values)
        query.bindValue(k++, value);
    insertSuccessfully =query.exec();
    if (insertSuccessfully)
    {
        emit MyDb::searchResultsInsertedSig(insertTracks);
    }
    this->searchResultDb_.close();
    return insertSuccessfully;
}

QString MyDb::searchResultsTableName() const
{
    return searchResultsTableName_;
}

