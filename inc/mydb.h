#ifndef MYDB_HPP
#define MYDB_HPP
#include <QSqlDatabase>
#include <QVariantMap>

enum class DbKeysIndex : int
{
    TRACK_ID = 0,
    TITLE = 1,
    ARTIST = 2,
    ALBUM =3,
    URL = 4
};

typedef QMap<DbKeysIndex, QString> TrackTableEntry;

static const TrackTableEntry TrackTableEntryKeyMap_ =
{
    {DbKeysIndex::TRACK_ID, "trackid"},
    {DbKeysIndex::TITLE, "trackname"},
    {DbKeysIndex::ARTIST, "artist"},
    {DbKeysIndex::ALBUM, "album"},    
    {DbKeysIndex::URL, "preview"},
};

class MyDb : public QObject
{
    Q_OBJECT
public:
    MyDb();
    bool openTracksTable();
    QString tracksTableName() const;
    bool addTrack(const QVariantMap &insertTracks);
    bool delTrack(const QVariantMap &delTrack);
    bool createSearchTable();
    bool addSearchResults(const QVariantMap &insertTracks);
    QString searchResultsTableName() const;
    QSqlDatabase searchResultDb() const ;
    QSqlDatabase playListDb()const;
public slots:
    void newSearchResultReceivedSlot(QList<QVariantMap> * list);
signals:
    void playlistChangedSig(const QVariantMap & inserted);
    void searchResultsInsertedSig(QList<QVariantMap> * list);

private:
    const QString tracksTableName_ = "tracks";
    const QString playlistDBName_ = "playlist.db";
    const QString searchResultsDBName_ = ":memory:";
    const QString searchResultsTableName_ = "searchresults";
    QSqlDatabase playListDb_;
    QSqlDatabase searchResultDb_;
};

#endif // MYDB_HPP
