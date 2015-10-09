#include "mainwindow.h"
#include <QApplication>

// TODO: add an oportunity to insert irregular verbs

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
