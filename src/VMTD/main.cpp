#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EnRunType runType = EnRunType::STANDARD;

    if (a.arguments().contains("-tray"))
    {
        runType = EnRunType::TRAY_MODE;
        a.setQuitOnLastWindowClosed(false);
    }
    else if (a.arguments().contains("-quickstart"))
    {
        runType = EnRunType::QUICK_START;
    }

    MainWindow w(nullptr, runType);
    w.show();

    return a.exec();
}
