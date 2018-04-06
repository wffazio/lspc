#include <QtWidgets>
#include <QVBoxLayout>
#include "inc/viewmodel.h"
#include "inc/view.h"
#include "inc/spotifywebapi.h"
#include "inc/mydb.h"

/*---------------------------------------------------------------------------*/
MyView::MyView( QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout(parent);

    tabsInstance_ = new ViewModel(parent);
    mainLayout->addWidget(tabsInstance_);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle(tr("Local Spotify Playlist Creator"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered,
            this, &MyView::about);



}


/*---------------------------------------------------------------------------*/
void MyView::about()
{
    QMessageBox::about(this, tr("Local Spotify Playlist Creator"),
            tr("<p>The <b>Local Spotify Playlist Creator</b> allows to create"
               " a local playlist based on Spotify Web API</p>"));
}


