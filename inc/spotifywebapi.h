#ifndef SPOTIFYWEBAPI_H
#define SPOTIFYWEBAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QTimer>
#include <QString>

#define SPOTIFY_API_BASE_URL "https://api.spotify.com"
#define SPOTIFY_API_VER "/v1"
#define SPOTIFY_API_SEARCH "/search?q="
#define SPOTIFY_GET_USERDATA_EP "/me"

#define SPOTIFY_API_SEARCH_ARTIST "type=artist&limit=2"
#define SPOTIFY_API_SEARCH_ALBUM  "type=album&limit=3"
#define SPOTIFY_API_SEARCH_TRACK  "type=track&limit=10"


#define SPOTIFY_API_CALL(call) SPOTIFY_API_BASE_URL SPOTIFY_API_VER call
#define SPOTIFY_SEARCH_CALL(q,type) SPOTIFY_API_BASE_URL SPOTIFY_API_VER SPOTIFY_API_SEARCH q "&" type

enum SpotifyWebApiRequestType
{
    spotifyWebApiRequestTypeServerStatus,
    spotifyWebApiRequestTypeAction,
    spotifyWebApiRequestTypeAlbumSearch,
    spotifyWebApiRequestTypeTrackSearch,
    spotifyWebApiRequestTypeArtistSearch,
    spotifyWebApiRequestTypePlay
};

struct SpotifyWebApiRequest
{
    SpotifyWebApiRequestType   RequestType;
    QNetworkRequest      Request;
};


class SpotifyWebApi: public QObject
{
    Q_OBJECT
public:
    SpotifyWebApi();
    bool SpotifyWebApiSearch(QString what, SpotifyWebApiRequestType type);

private:
    QString *token_ = nullptr;
    QTimer statusTimer_;
    QNetworkAccessManager networkAccessManager_;
    QMap<QNetworkReply*, SpotifyWebApiRequest> sentRequests_;
    QList<QVariantMap> * searchResults_ = nullptr;
    void onNetworkReplyReceived_(QNetworkReply *reply);
    void requestServerStatus_();
    void queueRequest_(const SpotifyWebApiRequest &request);
    void parseSearchResultReceived_(QByteArray *replyData);
signals:
    void newSearchResultReceivedSig(QList<QVariantMap> *);
public slots:
    void processSearchRequest(QString track=nullptr, QString artist = nullptr, QString album=nullptr);
    void storeToken(QString token);
    void playSlot();
};

#endif // SPOTIFYWEBAPI_H
