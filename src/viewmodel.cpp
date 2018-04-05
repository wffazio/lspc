#include "inc/viewmodel.h"
#include "inc/playercontrols.h"
#include "inc/view.h"
#include <QHeaderView>
#include <QPushButton>


#define PLAYLIST_MINIMUM_WIDTH 500


/*---------------------------------------------------------------------------*/
ViewModel::ViewModel(QWidget *parent)
{    
    QWidget *accountTabWdg = new QWidget(parent);
    this->cdb_ = new MyDb;
    if (!cdb_->openTracksTable() || !cdb_->createSearchTable())
    {
        qDebug() << "Db NOT connected";
        return;// EXIT_FAILURE;
    }

    qDebug() << "Db connected";
    currentAuthentication_ = new SpotifyAppAuthentication();
    currentAuthentication_->authenticate();

    this->spotifyApis_ = new SpotifyWebApi();

    playlistTableModel_ = createTableModelForPlaylist_(parent);
    player_ = new PlayerControls();
    this->addTab(createPlaylistTab_(playlistTableModel_),tr("Player"));
    QVBoxLayout *accountBoxLayout = new QVBoxLayout;

    activeUserWdg_ = new QLabel("Logged As: ");
    accountBoxLayout->addWidget(activeUserWdg_,1,Qt::AlignTop);

    accountTabWdg->setLayout(accountBoxLayout);
    this->addTab(createSearchTab_(parent),tr("Search"));
    this->addTab(accountTabWdg,tr("Account"));
    connect(this,QTabWidget::currentChanged,this,ViewModel::currentTabChangedSlot_);    
    connect(player_, &PlayerControls::playSig,this->spotifyApis_, &SpotifyWebApi::playSlot);
    connect(player_, &PlayerControls::pauseSig,this->spotifyApis_, &SpotifyWebApi::playSlot);
    connect(currentAuthentication_, &SpotifyAppAuthentication::userDataReceivedSig,this, &ViewModel::updateTabsWithUserDataSlot);
    connect(spotifyApis_, &SpotifyWebApi::newSearchResultReceivedSig,this->cdb_,&MyDb::newSearchResultReceivedSlot);
    connect(cdb_, &MyDb::searchResultsInsertedSig,this,&ViewModel::updateTabsWithSearchResultSlot);
    connect(cdb_, &MyDb::tracksInsertedSig,this,&ViewModel::updateTabsWithPlaylistSlot);
    connect(this,&ViewModel::startSearchTrackSig,this->spotifyApis_,&SpotifyWebApi::processSearchRequest);
    connect(currentAuthentication_, &SpotifyAppAuthentication::spotifyTokenReceivedSig,
            spotifyApis_, &SpotifyWebApi::storeToken);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(tabWidget);
    //this->setLayout(mainLayout);

    //setWindowTitle(tr("Local Spotify Playlist Creator"));
}


/*---------------------------------------------------------------------------*/
QSqlTableModel *
ViewModel::createTableModelForPlaylist_(QWidget *parent)
{
    QSqlTableModel * tableModelLocal = new QSqlTableModel(parent,cdb_->playListDb());

    tableModelLocal->setTable(cdb_->tracksTableName());
    tableModelLocal->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModelLocal->select();
    tableModelLocal->setHeaderData((int)DbKeysIndex::TRACK_ID, Qt::Horizontal, tr("ID"));
    tableModelLocal->setHeaderData((int)DbKeysIndex::TITLE, Qt::Horizontal, tr("Música"));
    tableModelLocal->setHeaderData((int)DbKeysIndex::ALBUM, Qt::Horizontal, tr("Álbum"));
    tableModelLocal->setHeaderData((int)DbKeysIndex::ARTIST, Qt::Horizontal, tr("Artista"));
    tableModelLocal->setHeaderData((int)DbKeysIndex::URL, Qt::Horizontal, tr("Preview URL"));
    return tableModelLocal;
}



/*---------------------------------------------------------------------------*/
QTableView *
ViewModel::createPlaylistView_(QSqlTableModel*table)
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
    trackViewWdgLocal->setColumnHidden((int)DbKeysIndex::TRACK_ID,true);
    trackViewWdgLocal->setColumnHidden((int)DbKeysIndex::URL,true);
    trackViewWdgLocal->sortByColumn((int)DbKeysIndex::TRACK_ID,Qt::AscendingOrder);
    //trackViewWdgLocal->setColumnWidth(1,150);
    trackViewWdgLocal->setWordWrap(false);
    QHeaderView * header = trackViewWdgLocal->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSortIndicatorShown(true);
    trackViewWdgLocal->setHorizontalHeader(header);

    //trackViewWdgLocal->horizontalHeader()->hide();
    QLocale locale = trackViewWdgLocal->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    trackViewWdgLocal->setLocale(locale);


    return trackViewWdgLocal;
}


/*---------------------------------------------------------------------------*/
QGroupBox *
ViewModel::createPlaylistViewBox_(QSqlTableModel*table)
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
ViewModel::createPlaylistTab_(QSqlTableModel*table)
{
    QWidget *localPlaylistTab = new QWidget;
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(createPlaylistViewBox_(table),0,0);
    gridLayout->addWidget(player_,1,0);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(0, 1);
    /*second column is empty yet*/
    gridLayout->setColumnMinimumWidth(0,PLAYLIST_MINIMUM_WIDTH);
    gridLayout->setColumnMinimumWidth(1,200);

    localPlaylistTab->setLayout(gridLayout);
    return localPlaylistTab;
}


/*---------------------------------------------------------------------------*/
QWidget *
ViewModel::createSearchTab_(QWidget *parent)
{
    QWidget * searchTabWdg=new QWidget;
    QVBoxLayout *tabLayout = new QVBoxLayout;
    QPushButton *searchButton = new QPushButton;
    QGroupBox * searchBox = new QGroupBox(QObject::tr("Search:"));
    QGroupBox * resultBox = new QGroupBox(QObject::tr("Results:"));
    QHBoxLayout *filtersContentLayout = new QHBoxLayout;
    QTableView * searchResultsWdg = new QTableView;
    QVBoxLayout * resultsBoxLayout = new QVBoxLayout;

    trackContentToSearch_ = new QLineEdit();
    albumContentToSearch_ = new QLineEdit();
    artistContentToSearch_ = new QLineEdit();
    trackContentToSearch_->setPlaceholderText(QObject::tr("(track title)"));
    albumContentToSearch_->setPlaceholderText(QObject::tr("(album title)"));
    artistContentToSearch_->setPlaceholderText(QObject::tr("(artist name)"));

    searchButton->setText(tr("Search..."));
    connect(searchButton, QPushButton::clicked,this,ViewModel::searchButtonClickedSlot_);

    filtersContentLayout->addWidget(trackContentToSearch_, 0, 0);
    filtersContentLayout->addWidget(albumContentToSearch_, 0, 0);
    filtersContentLayout->addWidget(artistContentToSearch_, 0, 0);
    filtersContentLayout->addWidget(searchButton,0,0);

    searchBox->setLayout(filtersContentLayout);

    searchResultsTableModel_ = new QSqlTableModel(parent,cdb_->searchResultDb());
    searchResultsTableModel_->setTable(cdb_->searchResultsTableName());
    searchResultsTableModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    searchResultsTableModel_->select();
    searchResultsTableModel_->setHeaderData((int)DbKeysIndex::TRACK_ID, Qt::Horizontal, tr("ID"));
    searchResultsTableModel_->setHeaderData((int)DbKeysIndex::TITLE, Qt::Horizontal, tr("Música"));
    searchResultsTableModel_->setHeaderData((int)DbKeysIndex::ALBUM, Qt::Horizontal, tr("Álbum"));
    searchResultsTableModel_->setHeaderData((int)DbKeysIndex::ARTIST, Qt::Horizontal, tr("Artista"));
    searchResultsTableModel_->setHeaderData((int)DbKeysIndex::URL, Qt::Horizontal, tr("Preview URL"));

    searchResultsWdg->setModel(searchResultsTableModel_);
    searchResultsWdg->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchResultsWdg->setSortingEnabled(true);
    searchResultsWdg->setSelectionBehavior(QAbstractItemView::SelectRows);
    searchResultsWdg->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResultsWdg->setShowGrid(true);
    searchResultsWdg->setGridStyle(Qt::NoPen);
    searchResultsWdg->verticalHeader()->hide();
    searchResultsWdg->setAlternatingRowColors(true);
    searchResultsWdg->setColumnHidden((int)DbKeysIndex::TRACK_ID,true);
    searchResultsWdg->setColumnHidden((int)DbKeysIndex::URL,true);
    searchResultsWdg->sortByColumn((int)DbKeysIndex::TRACK_ID,Qt::AscendingOrder);
    //trackViewWdgLocal->setColumnWidth(1,150);
    searchResultsWdg->setWordWrap(false);
    QHeaderView * header = searchResultsWdg->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSortIndicatorShown(true);
    searchResultsWdg->setHorizontalHeader(header);

    //trackViewWdgLocal->horizontalHeader()->hide();
    QLocale locale = searchResultsWdg->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    searchResultsWdg->setLocale(locale);
    connect(searchResultsWdg, QTableView::doubleClicked,this,ViewModel::addSelectionToPlaylistSlot_);

    resultsBoxLayout->addWidget(searchResultsWdg);
    resultBox->setLayout(resultsBoxLayout);


    tabLayout->addWidget(searchBox);
    tabLayout->addWidget(resultBox);

    searchTabWdg->setLayout(tabLayout);

    return searchTabWdg;
}


/*---------------------------------------------------------------------------*/
void
ViewModel::currentTabChangedSlot_(int index)
{
    qDebug() << "Tab has changed:" << index;
    this->currentWidget()->repaint();
}


/*---------------------------------------------------------------------------*/
void ViewModel::updateTabsWithUserDataSlot(QString userId, QString userName)
{
    activeUserWdg_->setText("Logged as: " +userName +"\n" + userId);
}


/*---------------------------------------------------------------------------*/
void ViewModel::updateTabsWithPlaylistSlot(const QVariantMap & track)
{
    (void)track;
    playlistTableModel_->select();
}


/*---------------------------------------------------------------------------*/
void ViewModel::updateTabsWithSearchResultSlot(QList<QVariantMap> * list)
{
    (void)list;
    searchResultsTableModel_->select();
}


/*---------------------------------------------------------------------------*/
void ViewModel::searchButtonClickedSlot_()
{
    QString track(trackContentToSearch_->text());
    QString artist(artistContentToSearch_->text());
    QString album(albumContentToSearch_->text());
    //SpotifyWebApiSearch("Sepultura",SpotifyWebApiRequestType::spotifyWebApiRequestTypeTrackSearch);
    emit startSearchTrackSig(track,artist,album);
}



/*---------------------------------------------------------------------------*/
void ViewModel::addSelectionToPlaylistSlot_(const QModelIndex &index)
{
    qDebug() << index.row() <<"is the Row";
    qDebug() << index.data().toString() <<"is the data";
    qDebug() << searchResultsTableModel_->data(index).toStringList().join(": ") <<"is the table content";
    qDebug() << searchResultsTableModel_->index(index.row(),1) << "is column 1";
    qDebug() << searchResultsTableModel_->itemData(index).values() << "is itemData Values";
    //searchResultsTableModel_->selectRow(index.row());
    qDebug() <<
                searchResultsTableModel_->index(index.row(),(int)DbKeysIndex::TRACK_ID).data().toString() <<
                searchResultsTableModel_->index(index.row(),(int)DbKeysIndex::TITLE).data().toString() <<
                searchResultsTableModel_->index(index.row(),(int)DbKeysIndex::ARTIST).data().toString() <<
                searchResultsTableModel_->index(index.row(),(int)DbKeysIndex::ALBUM).data().toString() <<
                searchResultsTableModel_->index(index.row(),(int)DbKeysIndex::URL).data().toString() <<
                "is full";
    QString message("Do you want to insert<br><b>"
                    + searchResultsTableModel_->index(index.row(),
                                                      (int)DbKeysIndex::ARTIST)
                                                       .data().toString()
                    +  "- "
                    +searchResultsTableModel_->index(index.row(),
                                                       (int)DbKeysIndex::TITLE)
                                                       .data().toString()
                    +"</b><br>into your playlist?");
    QMessageBox::StandardButton btn = QMessageBox::question(this,
                                              tr("Add to Playlist"),
                                              message,
                                              QMessageBox::Yes|QMessageBox::No,
                                              QMessageBox::Yes);

    if (QMessageBox::Yes == btn)
    {
        emit
    }

}
