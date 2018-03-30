#ifndef SPOTIFYAPPAUTHENTICATION_H
#define SPOTIFYAPPAUTHENTICATION_H

#include <QOAuth2AuthorizationCodeFlow>


class SpotifyAppAuthentication : public QObject
{
    Q_OBJECT

public:
    SpotifyAppAuthentication();
    ~SpotifyAppAuthentication();
    bool authenticate();

    QString getUserName() const;

private:
    const QString clientId = "aaa";
    const QString clientSecret = "bbb";
    void updateUserData();
    QOAuth2AuthorizationCodeFlow spotifyAuth;
    bool isGranted;
    QString userName = "None";

private slots:
    void granted();
    void authStatusChanged (QAbstractOAuth::Status status);
};

#endif // SPOTIFYAPPAUTHENTICATION_H
