#include "inc/spotifywebapi.h"
#include "inc/playercontrols.h"

#include <QWidget>
#include <QDebug>

SpotifyWebApi::SpotifyWebApi(QWidget *parent): QWidget(parent)
{
    //connect(this, &PlayerControls::playSig, this, &SpotifyWebApi::play);
}


void SpotifyWebApi::play()
{
    qDebug() << "Playing";
}
