/*********************
 * 进入程序后的界面
 * ******************/

#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include "choosegame.h"
class MainScene : public QWidget
{
    Q_OBJECT
    ChooseGame* ChooseGameWidget = nullptr;
public:
    MainScene(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
signals:

};

#endif // MAINSCENE_H
