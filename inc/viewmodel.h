#ifndef VIEWMODEL_H
#define VIEWMODEL_H
#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include <QTableView>
#include "inc/viewmodel.h"
#include "inc/mydb.h"
#include "inc/spotifyappauthentication.h"
#include "inc/playercontrols.h"
#include "inc/spotifywebapi.h"


class ViewModel : public QTabWidget
{
    Q_OBJECT
public:
    ViewModel(QWidget *parent = 0);

private:    
    MyDb * cdb_ = nullptr;
    SpotifyAppAuthentication *currentAuthentication_;
    SpotifyWebApi * spotifyApis_;
    PlayerControls * player_;
    QLineEdit *trackContentToSearch_;
    QLineEdit *albumContentToSearch_;
    QLineEdit *artistContentToSearch_;
    QSqlTableModel *playlistTableModel_ = nullptr;
    QSqlTableModel *searchResultsTableModel_ = nullptr;
    QLabel * activeUserWdg_ = nullptr;
    QTableView *trackViewWdg_;
    QToolButton *deleteBtn_;
    QTableView * createPlaylistView_ (QSqlTableModel*);
    QSqlTableModel * createTableModelForPlaylist_(QWidget *parent);
    QWidget * createPlaylistTab_( QSqlTableModel*model);
    QGroupBox * createPlaylistViewBox_(QSqlTableModel*);
    QGroupBox * createPlayerControlsBox_();
    QWidget * createSearchTab_( QWidget *parent);
    QVariantMap processSelect_(const QModelIndex &index);

public slots:
    void updateTabsWithUserDataSlot(QString userId, QString userName);
    void updateTabsWithPlaylistSlot(const QVariantMap & track);
    void updateTabsWithSearchResultSlot(QList<QVariantMap> * list);

signals:
    void startSearchTrackSig(QString track="",QString artist = "",QString album="");

private slots:
    void processSelectAndPlayReqSlot_(const QModelIndex &index);
    void processSelectSlot_(const QModelIndex &index);
    void currentTabChangedSlot_(int index);
    void searchButtonClickedSlot_();
    void addSelectionToPlaylistSlot_(const QModelIndex &index);
    void deleteSelectedTrackSlot_();
    void processNextTrackSlot_();
    void processPrevTrackSlot_();
};

#endif // VIEWMODEL_H
