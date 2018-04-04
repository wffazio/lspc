#include <QDesktopServices>
#include <QDebug>
#include <QtNetworkAuth>
#include <QDebug>
#include "inc/spotifyappauthentication.h"
#include "inc/spotifywebapi.h"

#define ACCOUNTS_URL "https://accounts.spotify.com/"
#define SPOTIFY_API_URL "https://api.spotify.com/v1/"



/*---------------------------------------------------------------------------*/
bool SpotifyAppAuthentication::authenticate()
{
    quint16 port(8080);
    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    spotifyAuth_.setReplyHandler(replyHandler);
    spotifyAuth_.setAuthorizationUrl(QUrl(ACCOUNTS_URL "authorize"));
    spotifyAuth_.setAccessTokenUrl(QUrl(ACCOUNTS_URL "api/token"));
    spotifyAuth_.setClientIdentifier(clientId_);
    spotifyAuth_.setClientIdentifierSharedKey(clientSecret_);
    spotifyAuth_.setScope("user-read-private user-top-read "
                         "playlist-read-private playlist-modify-public "
                         "playlist-modify-private");

    this->connect(&spotifyAuth_,
                    &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
                    &QDesktopServices::openUrl);

    this->connect(&spotifyAuth_, &QOAuth2AuthorizationCodeFlow::statusChanged,
                    this, &SpotifyAppAuthentication::authStatusChangedSlot_);

    this->connect(&spotifyAuth_, &QOAuth2AuthorizationCodeFlow::granted,
                    this, &SpotifyAppAuthentication::grantedSlot_);

    spotifyAuth_.grant();

    userName_ = "Granting...";

    return  true;
}

/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::updateUserData_()
{
    QUrl u (SPOTIFY_API_CALL(SPOTIFY_GET_USERDATA_EP));
    auto reply = spotifyAuth_.get(u);

    userName_ = "Retrieving...";

    connect(reply, &QNetworkReply::finished,
                    [=]()
                    {
                        if (reply->error() != QNetworkReply::NoError)
                        {
                            qDebug() << "Error Retrieving Userdata" <<reply->errorString();
                        }
                        else
                        {
                            const auto data = reply->readAll();
                            qDebug(data);
                            const auto document = QJsonDocument::fromJson(data);
                            const auto root = document.object();
                            userName_ = root.value("display_name").toString();
                            userId_ = root.value("id").toString();
                            qDebug() << "Username: (id)" << userName_ <<"(" <<userId_ <<")";
                            emit userDataReceivedSig(userId_, userName_);
                            reply->deleteLater();
                        }
                    });
}



/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::grantedSlot_ ()
{
    qDebug()<< "Token: " << spotifyAuth_.token()  << " has granted access";
    qDebug()<< "RToken: " << spotifyAuth_.refreshToken()  << " has granted access";
    qDebug()<< "Expiren: " << spotifyAuth_.expirationAt().toString() << " has granted access";

    emit spotifyTokenReceivedSig(spotifyAuth_.token());

    isGranted_ = true;
    updateUserData_();
}


/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::authStatusChangedSlot_(QAbstractOAuth::Status status)
{
    QString s;
    if (status == QAbstractOAuth::Status::Granted)
    {        
        s = "granted";        
    }

    if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
    {
        userName_ = "Temporary";
        s = "temp credentials";
    }

    qDebug() << "Auth Status changed: " << s;
}

/*---------------------------------------------------------------------------*/
SpotifyAppAuthentication::SpotifyAppAuthentication()
{

}

/*---------------------------------------------------------------------------*/
SpotifyAppAuthentication::~SpotifyAppAuthentication()
{

}

/*---------------------------------------------------------------------------*/
QString SpotifyAppAuthentication::getUserName()
{
    return userName_;
}


