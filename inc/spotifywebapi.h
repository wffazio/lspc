#ifndef SPOTIFYWEBAPI_H
#define SPOTIFYWEBAPI_H

#include <QWidget>

class SpotifyWebApi: public QWidget
{
    Q_OBJECT
public:
    SpotifyWebApi(QWidget *parent = nullptr);

public slots:
    void play();
};

#endif // SPOTIFYWEBAPI_H
