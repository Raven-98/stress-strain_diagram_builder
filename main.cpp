#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qApp->setWindowIcon(QIcon(":/img/icon.png"));

    MainWindow win;
    win.show();

    return app.exec();
}
