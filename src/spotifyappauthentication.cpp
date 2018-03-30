#include <QDesktopServices>
#include <QDebug>
#include <QtNetworkAuth>
#include <QDebug>
#include "inc/spotifyappauthentication.h"

#define ACCOUNTS_URL "https://accounts.spotify.com/"
#define API_URL "https://api.spotify.com/v1/"

/*---------------------------------------------------------------------------*/
bool SpotifyAppAuthentication::authenticate()
{
    quint16 port(8080);
    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    spotifyAuth.setReplyHandler(replyHandler);
    spotifyAuth.setAuthorizationUrl(QUrl(ACCOUNTS_URL "authorize"));
    spotifyAuth.setAccessTokenUrl(QUrl(ACCOUNTS_URL "api/token"));
    spotifyAuth.setClientIdentifier(clientId);
    spotifyAuth.setClientIdentifierSharedKey(clientSecret);
    spotifyAuth.setScope("user-read-private user-top-read "
                         "playlist-read-private playlist-modify-public "
                         "playlist-modify-private");

    this->connect(&spotifyAuth,
                    &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
                    &QDesktopServices::openUrl);

    this->connect(&spotifyAuth, &QOAuth2AuthorizationCodeFlow::statusChanged,
                    this, &SpotifyAppAuthentication::authStatusChanged);

    this->connect(&spotifyAuth, &QOAuth2AuthorizationCodeFlow::granted,
                    this, &SpotifyAppAuthentication::granted);

    spotifyAuth.grant();

    return  true;
}

/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::updateUserData()
{
    QUrl u (API_URL "me");
    auto reply = spotifyAuth.get(u);

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
                            userName = root.value("display_name").toString();
                            qDebug() << "Username: " << userName;
                            reply->deleteLater();
                        }
                    });
}

/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::granted ()
{
    QString token = spotifyAuth.token();
    qDebug()<< "Token: " << token  << " has granted access";
    isGranted = true;
    updateUserData();
}


/*---------------------------------------------------------------------------*/
void SpotifyAppAuthentication::authStatusChanged(QAbstractOAuth::Status status)
{
    QString s;
    if (status == QAbstractOAuth::Status::Granted)
    {
        s = "granted";
    }

    if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
    {
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
