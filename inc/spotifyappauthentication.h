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
    QString getUserName();


private:
    const QString clientId = "e5a26b574c264d499339f7e4b95de71b";
    const QString clientSecret = "26ef8aa25043406b9fd233191b456379";
    void updateUserData();
    QOAuth2AuthorizationCodeFlow spotifyAuth;
    bool isGranted;
    QString userName = "None";

private slots:
    void granted();
    void authStatusChanged (QAbstractOAuth::Status status);

};

#endif // SPOTIFYAPPAUTHENTICATION_H
