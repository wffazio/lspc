#include "inc/playercontrols.h"
#include <QToolButton>
#include <QStyle>
#include <QBoxLayout>


#define DEFAULT_BUTTON_H (40)
#define DEFAULT_BUTTON_W (DEFAULT_BUTTON_H*3)


PlayerControls::PlayerControls(QWidget *parent): QWidget(parent)
{
    QHBoxLayout *controlBoxLayout = new QHBoxLayout;
    btnPlayPause_ = new QToolButton(this);
    btnPlayPause_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(btnPlayPause_, &QAbstractButton::clicked, this, &PlayerControls::playClickedSlot_);

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
    btnPlayPause_->setMinimumSize(DEFAULT_BUTTON_W,DEFAULT_BUTTON_H);

    controlBoxLayout->addWidget(btnPrev_);
    controlBoxLayout->addWidget(btnPlayPause_);
    controlBoxLayout->addWidget(btnNext_);

    //controlBoxLayout->setMargin(20);

    this->setLayout(controlBoxLayout);

}

void PlayerControls::playClickedSlot_()
{
    if (isPlaying_)
    {
        btnPlayPause_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        btnNext_->setEnabled(false);
        btnPrev_->setEnabled(false);
        emit pauseSig();
    }
    else
    {
        btnPlayPause_->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        btnNext_->setEnabled(true);
        btnPrev_->setEnabled(true);
        emit playSig();
    }
    isPlaying_ = !isPlaying_;
}
