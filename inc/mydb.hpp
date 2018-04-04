#ifndef MYDB_HPP
#define MYDB_HPP
#include <QSqlDatabase>
class MyDb
{
public:
    QString tracksTable;
    QSqlDatabase db;
    bool connect();
};

#endif // MYDB_HPP
