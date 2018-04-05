#ifndef TABBEDMAINWINDOW_H
#define TABBEDMAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "inc/tabbedwindow.h"
#include "inc/mydb.hpp"
#include "inc/spotifyappauthentication.h"
#include "inc/playercontrols.h"


class TabbedMainWindow : public QTabWidget
{
    Q_OBJECT
public:
    TabbedMainWindow(MyDb *cdb, QWidget *parent = 0);
    PlayerControls * player;


private:    
    QLineEdit *trackContentToSearch_;
    QLineEdit *albumContentToSearch_;
    QLineEdit *artistContentToSearch_;
    QSqlTableModel *playlistTableModel_ = nullptr;
    QSqlTableModel *searchResultsTableModel_ = nullptr;
    QLabel * activeUserWdg_ = nullptr;
    QTableView *trackViewWdg_;
    QTableView * createPlaylistView_ (QSqlTableModel*);
    QSqlTableModel * createTableModelForPlaylist_(MyDb *cdb);
    QWidget * createPlaylistTab_(PlayerControls *player, QSqlTableModel*model);
    QGroupBox * createPlaylistViewBox_(QSqlTableModel*);
    QGroupBox * createPlayerControlsBox_();
    QWidget * createSearchTab_(QString tablename);

public slots:
    void updateTabsWithUserDataSlot(QString userId, QString userName);
    void updateTabsWithPlaylistSlot(const QVariantMap & track);
    void updateTabsWithSearchResultSlot(QList<QVariantMap> * list);

signals:
    void startSearchTrackSig(QString*track=nullptr,QString*artist = nullptr,QString*album=nullptr);

private slots:
    void currentTabChangedSlot_(int index);
    void searchButtonClickedSlot_();
};

#endif // TABBEDMAINWINDOW_H
