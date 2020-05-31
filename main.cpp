#include "mainscene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene* mainWidget = new MainScene;
    mainWidget->setWindowTitle("OOP扑克小游戏");
    mainWidget->show();
    return a.exec();
}

//todo: 换一些好一点的UI图片
