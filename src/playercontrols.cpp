#include "inc/playercontrols.h"
#include <QToolButton>
#include <QStyle>
#include <QBoxLayout>
#include <QUrl>
#include <QDebug>


#define DEFAULT_BUTTON_H (40)
#define DEFAULT_BUTTON_W (DEFAULT_BUTTON_H*3)

/*---------------------------------------------------------------------------*/
PlayerControls::PlayerControls(QWidget *parent): QWidget(parent)
{
    QHBoxLayout *controlBoxLayout = new QHBoxLayout;
    btnPlayStop_ = new QToolButton(this);
    btnPlayStop_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(btnPlayStop_, &QAbstractButton::clicked, this, &PlayerControls::buttonPlayStopClickedSlot_);

    btnNext_ = new QToolButton(this);
    btnNext_->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(btnNext_, &QAbstractButton::clicked, this, &PlayerControls::nextClickedSig);

    btnPrev_ = new QToolButton(this);
    btnPrev_->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(btnPrev_, &QAbstractButton::clicked, this, &PlayerControls::prevClickedSig);

    btnNext_->setEnabled(false);
    btnPrev_->setEnabled(false);
    //btnNext_->adjustSize();
    btnNext_->setMinimumSize(DEFAULT_BUTTON_W,DEFAULT_BUTTON_H);
    btnPrev_->setMinimumSize(DEFAULT_BUTTON_W,DEFAULT_BUTTON_H);
    btnPlayStop_->setMinimumSize(DEFAULT_BUTTON_W,DEFAULT_BUTTON_H);

    controlBoxLayout->addWidget(btnPrev_);
    controlBoxLayout->addWidget(btnPlayStop_);
    controlBoxLayout->addWidget(btnNext_);

    //controlBoxLayout->setMargin(20);

    this->setLayout(controlBoxLayout);

}


/*---------------------------------------------------------------------------*/
QVariantMap PlayerControls::currentTrack() const
{
    return currentTrack_;
}


/*---------------------------------------------------------------------------*/
void PlayerControls::setCurrentTrack(const QVariantMap &currentTrack)
{
    currentTrack_ = currentTrack;
}

/*---------------------------------------------------------------------------*/
void PlayerControls::playerPlaySlot()
{
    if ( !currentTrack_.isEmpty() )
    {
        btnPlayStop_->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
        btnNext_->setEnabled(true);
        btnPrev_->setEnabled(true);
        isPlaying_ = true;
        emit playSig(currentTrack_);
    }
}


/*---------------------------------------------------------------------------*/
void PlayerControls::playerStopSlot()
{
    btnPlayStop_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    btnNext_->setEnabled(false);
    btnPrev_->setEnabled(false);
    isPlaying_ =false;
    emit stopSig();
}

/*---------------------------------------------------------------------------*/
void PlayerControls::buttonPlayStopClickedSlot_()
{

    if (isPlaying_)
    {
        playerStopSlot();
    }
    else
    {
        playerPlaySlot();
    }
}


/*---------------------------------------------------------------------------*/
void PlayerControls::trackSelectedSlot_(QVariantMap& track)
{
    qDebug() << __func__;
}
