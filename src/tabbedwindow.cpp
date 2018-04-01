#include "inc/tabbedwindow.h"
#include "inc/playercontrols.h"
#include "inc/appmainwindow.h"

static QLabel * activeUserWdg_;
static int accountTabIndex_=-1;

/*---------------------------------------------------------------------------*/
QSqlTableModel *
TabbedMainWindow::createTableModelForPlaylist_(MyDb &cdb)
{
    QSqlTableModel * tableModelLocal = new QSqlTableModel;
    tableModelLocal->setTable(cdb.activeTable);
    tableModelLocal->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModelLocal->select();
    return tableModelLocal;
}



/*---------------------------------------------------------------------------*/
TabbedMainWindow::TabbedMainWindow(MyDb &cdb, QWidget *parent)
{    
    QWidget *searchTabWdg = new QWidget(parent);
    QWidget *accountTabWdg = new QWidget(parent);

    tableModel_ = createTableModelForPlaylist_(cdb);
    player = new PlayerControls();
    currentAuthentication_ = new SpotifyAppAuthentication();
    currentAuthentication_->authenticate();

    this->addTab(createPlaylistTab_(player,tableModel_),tr("Player"));
    QVBoxLayout *accountBoxLayout = new QVBoxLayout;

    activeUserWdg_ = new QLabel("Logged As: ");
    accountBoxLayout->addWidget(activeUserWdg_,1,Qt::AlignTop);

    accountTabWdg->setLayout(accountBoxLayout);


    this->addTab(searchTabWdg,tr("Search"));
    accountTabIndex_=this->addTab(accountTabWdg,tr("Account"));
    connect(this,QTabWidget::currentChanged,this,TabbedMainWindow::currentTabChangedSlot);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(tabWidget);
    //this->setLayout(mainLayout);

    //setWindowTitle(tr("Local Spotify Playlist Creator"));
}

/*---------------------------------------------------------------------------*/
void
TabbedMainWindow::currentTabChangedSlot(int index)
{
    if (index == accountTabIndex_)
    {
        activeUserWdg_->setText("Logged as: " + currentAuthentication_->getUserName());
    }

}



/*---------------------------------------------------------------------------*/
QTableView *
TabbedMainWindow::createPlaylistView_(QSqlTableModel*table)
{
    QTableView *trackViewWdgLocal = new QTableView;
    trackViewWdgLocal->setModel(table);
    trackViewWdgLocal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trackViewWdgLocal->setSortingEnabled(true);
    trackViewWdgLocal->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackViewWdgLocal->setSelectionMode(QAbstractItemView::SingleSelection);
    trackViewWdgLocal->setShowGrid(false);
    trackViewWdgLocal->verticalHeader()->hide();
    trackViewWdgLocal->setAlternatingRowColors(true);
    trackViewWdgLocal->setColumnHidden(0,true);
    trackViewWdgLocal->horizontalHeader()->hide();
    QLocale locale = trackViewWdgLocal->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    trackViewWdgLocal->setLocale(locale);
    return trackViewWdgLocal;
}


/*---------------------------------------------------------------------------*/
QGroupBox *
TabbedMainWindow::createPlaylistViewBox_(QSqlTableModel*table)
{
    static QTableView *trackViewWdg_;
    static QLineEdit *filterWdg_;
    QVBoxLayout *tracklistBoxLayout = new QVBoxLayout;
    QGroupBox * tracksListBox = new QGroupBox(QObject::tr("Faixas"));

    filterWdg_ = new QLineEdit();
    filterWdg_->setPlaceholderText(QObject::tr("(filter)"));
    trackViewWdg_ = createPlaylistView_(table);

    tracklistBoxLayout->addWidget(filterWdg_, 0, 0);
    tracklistBoxLayout->addWidget(trackViewWdg_, 0, 0);
    tracksListBox->setLayout(tracklistBoxLayout);
    return tracksListBox;
}


/*---------------------------------------------------------------------------*/
QWidget *
TabbedMainWindow::createPlaylistTab_(PlayerControls *player,
                                     QSqlTableModel*table)
{
    QWidget *localPlaylistTab = new QWidget;
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(createPlaylistViewBox_(table),0,0);
    gridLayout->addWidget(player,1,0);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(0, 1);
    /*second column is empty yet*/
    gridLayout->setColumnMinimumWidth(0, 400);
    gridLayout->setColumnMinimumWidth(1,200);

    localPlaylistTab->setLayout(gridLayout);
    return localPlaylistTab;
}

