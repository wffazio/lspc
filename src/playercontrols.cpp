#include "inc/playercontrols.h"
#include <QToolButton>
#include <QStyle>
#include <QBoxLayout>

PlayerControls::PlayerControls(QWidget *parent): QWidget(parent)
{
    QHBoxLayout *controlBoxLayout = new QHBoxLayout;
    btnPlayPause = new QToolButton(this);
    btnPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(btnPlayPause, &QAbstractButton::clicked, this, &PlayerControls::playClickedSlot);

    btnNext = new QToolButton(this);
    btnNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(btnNext, &QAbstractButton::clicked, this, &PlayerControls::nextClickedSig);

    btnPrev = new QToolButton(this);
    btnPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(btnPrev, &QAbstractButton::clicked, this, &PlayerControls::prevClickedSig);

    controlBoxLayout->addWidget(btnPrev,0,Qt::AlignLeft|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnPlayPause,1,Qt::AlignCenter|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnNext,0,Qt::AlignRight|Qt::AlignBottom);

    controlBoxLayout->setMargin(20);

    this->setLayout(controlBoxLayout);

}

void PlayerControls::playClickedSlot()
{
    if (isPlaying)
    {
        emit pauseSig();
    }
    else
    {
        emit playSig();
    }
    isPlaying = !isPlaying;
}
