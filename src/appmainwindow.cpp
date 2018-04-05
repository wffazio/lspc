#include <QtWidgets>
#include <QVBoxLayout>
#include "inc/tabbedwindow.h"
#include "inc/appmainwindow.h"
#include "inc/spotifywebapi.h"
#include "inc/mydb.hpp"

/*---------------------------------------------------------------------------*/
AppMainWindow::AppMainWindow(MyDb &cdb, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout(parent);

    currentAuthentication_ = new SpotifyAppAuthentication();
    currentAuthentication_->authenticate();
    this->cdb_=&cdb;
    tabsInstance_ = new TabbedMainWindow(cdb,parent);
    this->spotifyApis_ = new SpotifyWebApi();


    connect(tabsInstance_->player, &PlayerControls::playSig,this->spotifyApis_, &SpotifyWebApi::playSlot);
    connect(tabsInstance_->player, &PlayerControls::pauseSig,this->spotifyApis_, &SpotifyWebApi::playSlot);
    connect(currentAuthentication_, &SpotifyAppAuthentication::userDataReceivedSig,tabsInstance_, &TabbedMainWindow::updateTabsWithUserDataSlot);
    connect(spotifyApis_, &SpotifyWebApi::newSearchResultReceivedSig,this->cdb_,&MyDb::newSearchResultReceivedSlot);
    connect(cdb_, &MyDb::searchResultsInsertedSig,tabsInstance_,&TabbedMainWindow::updateTabsWithSearchResultSlot);
    connect(cdb_, &MyDb::tracksInsertedSig,tabsInstance_,&TabbedMainWindow::updateTabsWithPlaylistSlot);
    connect(tabsInstance_,&TabbedMainWindow::startSearchTrackSig,this->spotifyApis_,&SpotifyWebApi::processSearchRequest);


    mainLayout->addWidget(tabsInstance_);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle(tr("Local Spotify Playlist Creator"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered,
            this, &AppMainWindow::about);

    connect(currentAuthentication_, &SpotifyAppAuthentication::spotifyTokenReceivedSig,
            spotifyApis_, &SpotifyWebApi::storeToken);

}


/*---------------------------------------------------------------------------*/
void AppMainWindow::about()
{
    QMessageBox::about(this, tr("Local Spotify Playlist Creator"),
            tr("<p>The <b>Local Spotify Playlist Creator</b> allows to create"
               " a local playlist based on Spotify Web API</p>"));
}


