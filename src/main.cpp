#include "inc/view.h"
#include "inc/mydb.h"
#include <QApplication>
#include <QDebug>
#include "inc/viewmodel.h"


/*---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyView w;
    w.show();

    return app.exec();
}
