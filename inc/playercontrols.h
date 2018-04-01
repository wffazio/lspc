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
    bool isPlaying = false;
    QToolButton *btnPlayPause = nullptr;
    QToolButton *btnNext= nullptr;
    QToolButton *btnPrev= nullptr;

signals:
    void nextClickedSig();
    void prevClickedSig();
    void playSig();
    void pauseSig();

private slots:
    void playClickedSlot();
};

#endif // PLAYERCONTROLS_H
