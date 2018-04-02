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
    const QString clientId_ = "xxx";
    const QString clientSecret_ = "xxx";
    void updateUserData_();
    QOAuth2AuthorizationCodeFlow spotifyAuth_;
    bool isGranted_;
    QString userName_ = "None";
    QString userId_ = "";

private slots:
    void grantedSlot_();
    void authStatusChangedSlot_ (QAbstractOAuth::Status status);

signals:
    void userDataReceivedSig(QString userId, QString userName);


};

#endif // SPOTIFYAPPAUTHENTICATION_H
