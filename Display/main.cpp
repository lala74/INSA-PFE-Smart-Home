#include <QApplication>

#include "display.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    w.showFullScreen();
    //    w.show();
    return a.exec();
}
