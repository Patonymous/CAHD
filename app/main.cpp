#include "mainwindow/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
#if defined(_WIN32)
    // never use ANGLE on Windows, since OCCT 3D Viewer does not expect this
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    // QCoreApplication::setAttribute (Qt::AA_UseOpenGLES);
#endif
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
