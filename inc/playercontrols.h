#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H
#include<QWidget>
#include <QToolButton>
#include <QModelIndex>

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

signals:
    void nextClickedSig();
    void prevClickedSig();
    void playSig(QVariantMap& );
    void stopSig();

private slots:
    void trackSelectedSlot_(QVariantMap& track);
    void buttonPlayStopClickedSlot_();
public slots:
    void playerPlaySlot();
    void playerStopSlot();
};

#endif // PLAYERCONTROLS_H
