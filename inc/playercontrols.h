#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H
#include<QWidget>
#include <QToolButton>
#include <QModelIndex>
#include <QMediaPlayer>

class PlayerControls : public QWidget
{
    Q_OBJECT
public:
    PlayerControls(QWidget *parent = nullptr);
    QVariantMap currentTrack() const;
    void setCurrentTrack(const QVariantMap &currentTrack);

private:
    bool isPlaying_ = false;
    QToolButton *btnPlayStop_ = nullptr;
    QToolButton *btnNext_= nullptr;
    QToolButton *btnPrev_= nullptr;
    QVariantMap currentTrack_;
    QMediaPlayer * multimedia_ = nullptr;

signals:
    void nextClickedSig();
    void prevClickedSig();
    void playSig(QVariantMap& );
    void stopSig();

private slots:
    void buttonPlayStopClickedSlot_();
    void multimediaChangedStateSlot_(QMediaPlayer::State state);
public slots:
    void playerPlaySlot();
    void playerStopSlot();
};

#endif // PLAYERCONTROLS_H
