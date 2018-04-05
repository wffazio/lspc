#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "inc/mydb.h"
#include "inc/spotifywebapi.h"
#include "inc/viewmodel.h"
#include "inc/playercontrols.h"

class MyView : public QMainWindow
{
    Q_OBJECT

public:
    MyView(QWidget *parent = 0);

private:    
    ViewModel * tabsInstance_;
    void about();

};

#endif // MAINWINDOW_H
