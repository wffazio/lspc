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
    SpotifyWebApi * spotifyApis;
    TabbedMainWindow * tabsInstance;
    void about();
};

#endif // MAINWINDOW_H
