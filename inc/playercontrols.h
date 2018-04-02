#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H
#include<QWidget>
#include <QToolButton>

class PlayerControls : public QWidget
{
    Q_OBJECT
public:
    PlayerControls(QWidget *parent = nullptr);
private:
    bool isPlaying_ = false;
    QToolButton *btnPlayPause_ = nullptr;
    QToolButton *btnNext_= nullptr;
    QToolButton *btnPrev_= nullptr;

signals:
    void nextClickedSig();
    void prevClickedSig();
    void playSig();
    void pauseSig();

private slots:
    void playClickedSlot_();
};

#endif // PLAYERCONTROLS_H
