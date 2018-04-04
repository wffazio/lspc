#include "inc/tabbedwindow.h"
#include "inc/playercontrols.h"
#include "inc/appmainwindow.h"
#include <QHeaderView>


#define PLAYLIST_MINIMUM_WIDTH 500

/*---------------------------------------------------------------------------*/
QSqlTableModel *
TabbedMainWindow::createTableModelForPlaylist_(MyDb *cdb)
{
    QSqlTableModel * tableModelLocal = new QSqlTableModel;

    tableModelLocal->setTable(cdb->tracksTableName());
    tableModelLocal->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModelLocal->select();
    tableModelLocal->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModelLocal->setHeaderData(1, Qt::Horizontal, tr("Música"));
    tableModelLocal->setHeaderData(2, Qt::Horizontal, tr("Álbum"));
    tableModelLocal->setHeaderData(3, Qt::Horizontal, tr("Artista"));
    tableModelLocal->setHeaderData(4, Qt::Horizontal, tr("Preview URL"));
    return tableModelLocal;
}



/*---------------------------------------------------------------------------*/
TabbedMainWindow::TabbedMainWindow(MyDb *cdb, QWidget *parent)
{    
    QWidget *searchTabWdg = new QWidget(parent);
    QWidget *accountTabWdg = new QWidget(parent);

    playlistTableModel_ = createTableModelForPlaylist_(cdb);
    player = new PlayerControls();
    this->addTab(createPlaylistTab_(player,playlistTableModel_),tr("Player"));
    QVBoxLayout *accountBoxLayout = new QVBoxLayout;

    activeUserWdg_ = new QLabel("Logged As: ");
    accountBoxLayout->addWidget(activeUserWdg_,1,Qt::AlignTop);

    accountTabWdg->setLayout(accountBoxLayout);


    this->addTab(searchTabWdg,tr("Search"));
    this->addTab(accountTabWdg,tr("Account"));
    connect(this,QTabWidget::currentChanged,this,TabbedMainWindow::currentTabChangedSlot_);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(tabWidget);
    //this->setLayout(mainLayout);

    //setWindowTitle(tr("Local Spotify Playlist Creator"));
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
    trackViewWdgLocal->setShowGrid(true);
    trackViewWdgLocal->setGridStyle(Qt::NoPen);
    trackViewWdgLocal->verticalHeader()->hide();
    trackViewWdgLocal->setAlternatingRowColors(true);
    trackViewWdgLocal->setColumnHidden(0,true);
    trackViewWdgLocal->setColumnHidden(4,true);
    //trackViewWdgLocal->setColumnWidth(1,150);
    trackViewWdgLocal->setWordWrap(false);
    QHeaderView * header = trackViewWdgLocal->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    trackViewWdgLocal->setHorizontalHeader(header);

    //trackViewWdgLocal->horizontalHeader()->hide();
    QLocale locale = trackViewWdgLocal->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    trackViewWdgLocal->setLocale(locale);
    return trackViewWdgLocal;
}


/*---------------------------------------------------------------------------*/
QGroupBox *
TabbedMainWindow::createPlaylistViewBox_(QSqlTableModel*table)
{
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
    gridLayout->setColumnMinimumWidth(0,PLAYLIST_MINIMUM_WIDTH);
    gridLayout->setColumnMinimumWidth(1,200);

    localPlaylistTab->setLayout(gridLayout);
    return localPlaylistTab;
}


/*---------------------------------------------------------------------------*/
void
TabbedMainWindow::currentTabChangedSlot_(int index)
{
    qDebug() << "Tab has changed:" << index;
    this->currentWidget()->repaint();
}


/*---------------------------------------------------------------------------*/
void TabbedMainWindow::updateTabsWithUserDataSlot(QString userId, QString userName)
{
    activeUserWdg_->setText("Logged as: " +userName +"\n" + userId);
}


/*---------------------------------------------------------------------------*/
void TabbedMainWindow::updateTabsWithPlaylistSlot(const QVariantMap & track)
{
    qDebug() << track;
    trackViewWdg_->repaint();
}


/*---------------------------------------------------------------------------*/
void TabbedMainWindow::updateTabsWithSearchResultSlot(QList<QVariantMap> * list)
{
    for (int j =0; list->size() > j; j++)
    {
        QVariantMap item = list->at(j);
        QVariantMap::iterator it ;
        QString out;
        for (it = item.begin(); it != item.end(); ++it)
        {
            out.append(it.key().toLocal8Bit());
            out.append(" ");
            out.append(it.value().toString());
            out.append(" ");
        }
        qDebug()<<out;
    }
}
