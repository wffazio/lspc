#include "inc/mainwindow.h"
#include <QtWidgets>


MainWindow::MainWindow(MyDb &cdb, QWidget *parent) :
    QMainWindow(parent)
{
    model = new QSqlTableModel;
    trackView = new QTableView;

    model->setTable(cdb.activeTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    QGroupBox * tracksListBox = new QGroupBox(tr("Faixas"));

    QGridLayout *gridLayout = new QGridLayout;
    filter = new QLineEdit("(filter)");

    trackView->setModel(model);
    trackView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trackView->setSortingEnabled(true);
    trackView->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackView->setSelectionMode(QAbstractItemView::SingleSelection);
    trackView->setShowGrid(false);
    trackView->verticalHeader()->hide();
    trackView->setAlternatingRowColors(true);
    trackView->setColumnHidden(0,true);
    trackView->horizontalHeader()->hide();


    QLocale locale = trackView->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    trackView->setLocale(locale);

    QVBoxLayout *tracklistBoxLayout = new QVBoxLayout;
    tracklistBoxLayout->addWidget(trackView, 0, 0);
    tracksListBox->setLayout(tracklistBoxLayout);

    gridLayout->addWidget(filter,0,0);
    gridLayout->addWidget(tracksListBox,1,0);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(0, 2);
    gridLayout->setColumnMinimumWidth(0, 400);
    gridLayout->setColumnMinimumWidth(1,200);

    QHBoxLayout *controlBoxLayout = new QHBoxLayout;
    QGroupBox * controlsBox = new QGroupBox();
    QPushButton *btnPlay = new QPushButton("Play/Pause");
    QPushButton *btnNext = new QPushButton("Next");
    QPushButton *btnPrev = new QPushButton("Prev");

    btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    btnNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    btnPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    btnNext->setDisabled(true);
    btnPrev->setDisabled(true);

    controlBoxLayout->addWidget(btnPrev,0,Qt::AlignLeft|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnPlay,1,Qt::AlignCenter|Qt::AlignBottom);
    controlBoxLayout->addWidget(btnNext,0,Qt::AlignRight|Qt::AlignBottom);

    controlBoxLayout->setMargin(20);

    controlsBox->setLayout(controlBoxLayout);


    gridLayout->addWidget(controlsBox,2,0);

    QWidget *widget = new QWidget;
    widget->setLayout(gridLayout);
    setCentralWidget(widget);

    setWindowTitle(tr("Local Spotify Playlist Creator"));

}

