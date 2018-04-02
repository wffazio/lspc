#include "inc/playercontrols.h"
#include <QToolButton>
#include <QStyle>
#include <QBoxLayout>

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

    controlBoxLayout->addWidget(btnPrev_,0,Qt::AlignLeft|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnPlayPause_,1,Qt::AlignCenter|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnNext_,0,Qt::AlignRight|Qt::AlignBottom);

    controlBoxLayout->setMargin(20);

    this->setLayout(controlBoxLayout);

}

void PlayerControls::playClickedSlot_()
{
    if (isPlaying_)
    {
        emit pauseSig();
    }
    else
    {
        emit playSig();
    }
    isPlaying_ = !isPlaying_;
}
