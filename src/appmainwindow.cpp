#include <QtWidgets>
#include <QVBoxLayout>
#include "inc/tabbedwindow.h"
#include "inc/appmainwindow.h"
#include "inc/spotifywebapi.h"

/*---------------------------------------------------------------------------*/
AppMainWindow::AppMainWindow(MyDb &cdb, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout(parent);

    tabsInstance = new TabbedMainWindow(cdb,parent);
    this->spotifyApis = new SpotifyWebApi();

    connect(tabsInstance->player, &PlayerControls::playSig,this->spotifyApis, &SpotifyWebApi::play);
    connect(tabsInstance->player, &PlayerControls::pauseSig,this->spotifyApis, &SpotifyWebApi::play);

    mainLayout->addWidget(tabsInstance);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle(tr("Local Spotify Playlist Creator"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered,
            this, &AppMainWindow::about);

}


/*---------------------------------------------------------------------------*/
void AppMainWindow::about()
{
    QMessageBox::about(this, tr("Local Spotify Playlist Creator"),
            tr("<p>The <b>Local Spotify Playlist Creator</b> allows to create"
               " a local playlist based on Spotify Web API</p>"));
}
