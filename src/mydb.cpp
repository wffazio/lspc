#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "inc/mydb.h"

#undef CREATE_DUMMY_DB

/*---------------------------------------------------------------------------*/
MyDb::MyDb()
{

}


/*---------------------------------------------------------------------------*/
bool MyDb::openTracksTable()
{   
    this->playListDb_= QSqlDatabase::addDatabase("QSQLITE");

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
            QString createString= QString(
                                "create table %1 "
                                "(%2 integer PRIMARY KEY AUTOINCREMENT, "
                                "%3 varchar(64), "
                                "%4 varchar(64), "
                                "%5 varchar(64), "
                                "%6 varchar(256))")
                                .arg(this->tracksTableName_)
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::TRACK_ID])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::TITLE])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::ARTIST])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::ALBUM])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::URL]);

            query.exec(createString);
#ifdef CREATE_DUMMY_DB
            query.exec("insert into "+ this->tracksTableName_ +" (trackname,album,artist,preview) "
                       "values('The Mission','Hero','Van Canto','')");
            query.exec("insert into "+ this->tracksTableName_ +" (trackname,album,artist,preview) "
                       "values('Lifetime','Hero','Van Canto','')");
            query.exec("insert into "+ this->tracksTableName_ +" (trackname,album,artist,preview) "
                       "values('Sad But True','Metallica (Black Album)','Metallica','')");
#endif
        }
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
    if (!this->playListDb_.isOpen())
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
    QSqlQuery query(this->playListDb_);
    query.prepare(sqlQueryString);

    int k = 0;
    foreach (const QVariant &value, values)
        query.bindValue(k++, value);
    insertSuccessfully =query.exec();
    if (insertSuccessfully)
    {
        emit MyDb::playlistChangedSig(insertTracks);
    }
    return insertSuccessfully;

}


/*---------------------------------------------------------------------------*/
bool MyDb::createSearchTable()
{
    this->searchResultDb_ = QSqlDatabase::addDatabase(
                                              "QSQLITE",
                                              this->searchResultsDBName_);
    this->searchResultDb_.setDatabaseName(this->searchResultsDBName_);
    QSqlQuery query(this->searchResultDb_);
    bool ret = false;
    if (!this->searchResultDb_.open())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir ")+
                                          this->searchResultsTableName_,
                              QObject::tr("Isto pode ter sido causado"
                                          "por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    else
    {
        if (!this->searchResultDb_.tables().contains(this->searchResultsTableName_))
        {
            qDebug() << "First search DB creation";
            QString createString= QString(
                                "create table %1 "
                                "(%2 integer PRIMARY KEY AUTOINCREMENT, "
                                "%3 varchar(64), "
                                "%4 varchar(64), "
                                "%5 varchar(64), "
                                "%6 varchar(256))")
                                .arg(this->searchResultsTableName_)
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::TRACK_ID])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::TITLE])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::ARTIST])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::ALBUM])
                                .arg(TrackTableEntryKeyMap_[DbKeysIndex::URL]);

            query.exec(createString);
#ifdef CREATE_DUMMY_DB
            query.exec("insert into "+ this->searchResultsTableName_ +" (trackname,album,artist,preview) "
                       "values('The Mission2','Hero','Van Canto','')");
            query.exec("insert into "+ this->searchResultsTableName_ +" (trackname,album,artist,preview) "
                       "values('Lifetime2','Hero','Van Canto','')");
            query.exec("insert into "+ this->searchResultsTableName_ +" (trackname,album,artist,preview) "
                       "values('Sad But True2','Metallica (Black Album)','Metallica','')");
#endif
        }
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
    if (!this->searchResultDb_.isOpen())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir") +this->searchResultsTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query(this->searchResultDb_);

    QStringList strValues;
    QStringList fields = insertTracks.keys();
    QVariantList values = insertTracks.values();
    int totalFields = fields.size();
    for (int i = 0; i < totalFields; ++i)
        strValues.append("?");

    QString sqlQueryString = "insert into " + this->searchResultsTableName_ +
            " (" + QString(fields.join(",")) + ") values(" +
            QString(strValues.join(",")) + ")";

    query.prepare(sqlQueryString);

    int k = 0;
    foreach (const QVariant &value, values)
        query.bindValue(k++, value);
    insertSuccessfully =query.exec();
    return insertSuccessfully;
}


/*---------------------------------------------------------------------------*/
QString MyDb::searchResultsTableName() const
{
    return searchResultsTableName_;
}


/*---------------------------------------------------------------------------*/
QSqlDatabase MyDb::searchResultDb() const
{
    return searchResultDb_;
}


/*---------------------------------------------------------------------------*/
QSqlDatabase MyDb::playListDb() const
{
    return playListDb_;
}


/*---------------------------------------------------------------------------*/
void MyDb::newSearchResultReceivedSlot(QList<QVariantMap> * list)
{
    QSqlQuery query(this->searchResultDb_);
    query.exec("delete from "+this->searchResultsTableName_);
    query.finish();
    for (int j =0; list->size() > j; j++)
    {
        QVariantMap item = list->at(j);
        MyDb::addSearchResults(item);
    }
    emit MyDb::searchResultsInsertedSig(list);
}


/*---------------------------------------------------------------------------*/
bool MyDb::delTrack(const QVariantMap &delTrack)
{
    bool deleted;
    if (delTrack.isEmpty())
    {
        qDebug()<<QStringLiteral("Fatal error on deletion! "
                                 "The delTrack is empty!");
        return false;
    }
    if (!this->playListDb_.isOpen())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Não foi possível abrir ")+this->tracksTableName_,
                              QObject::tr("Isto pode ter sido causado por falta do SQLite.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return false;
    }

    QString sqlQueryString =  QString("delete from %1 where %2=%3")
                                     .arg(this->tracksTableName_)
                                        .arg(TrackTableEntryKeyMap_[DbKeysIndex::TRACK_ID])
                                        .arg(delTrack.value(TrackTableEntryKeyMap_[DbKeysIndex::TRACK_ID]).toString());
    QSqlQuery query(this->playListDb_);
    query.prepare(sqlQueryString);
    deleted =query.exec();
    if (deleted)
    {
        emit MyDb::playlistChangedSig(delTrack);
    }
    return deleted;
}
