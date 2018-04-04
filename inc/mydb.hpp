#ifndef MYDB_HPP
#define MYDB_HPP
#include <QSqlDatabase>
#include <QVariantMap>

enum class DbKeysIndex : uint8_t
{
    TRACK_ID = 0,
    TITLE = 1,
    ALBUM = 2,
    ARTIST = 3,
    URL = 4
};

typedef QMap<DbKeysIndex, QString> TrackTableEntry;

static const TrackTableEntry TrackTableEntryKeyMap =
{
    {DbKeysIndex::TRACK_ID, "trackid"},
    {DbKeysIndex::TITLE, "trackname"},
    {DbKeysIndex::ALBUM, "album"},
    {DbKeysIndex::ARTIST, "artist"},
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

signals:
    void tracksInsertedSig(const QVariantMap & inserted);

private:
    const QString tracksTableName_ = "tracks";
    const QString playlistDBName_ = "playlist.db";
    QSqlDatabase playListDb_;
};

#endif // MYDB_HPP
