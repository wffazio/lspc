#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "inc/mydb.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MyDb &db, QWidget *parent = 0);

private:
   QSqlTableModel *model;
   QTableView *trackView;
   QLineEdit *filter;

};

#endif // MAINWINDOW_H
