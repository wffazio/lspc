#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "inc/mydb.hpp"
#include "inc/spotifywebapi.h"
#include "inc/tabbedwindow.h"
#include "inc/playercontrols.h"

class AppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppMainWindow(MyDb &db, QWidget *parent = 0);

private:
    MyDb * cdb_ = nullptr;
    SpotifyAppAuthentication *currentAuthentication_;
    SpotifyWebApi * spotifyApis_;
    TabbedMainWindow * tabsInstance_;
    void about();

};

#endif // MAINWINDOW_H
