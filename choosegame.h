/*********************************
 * 选择游戏类型
 * ******************************/

#ifndef CHOOSEGAME_H
#define CHOOSEGAME_H

#include <QMainWindow>
#include <QWidget>
#include "landlordofflinedesk.h"
#include "blackjackofflinedesk.h"
class ChooseGame : public QWidget
{
    Q_OBJECT
    int GameType;  //游戏类型，0是单机，1是联机
public:
    ChooseGame(int gameType,QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);

signals:
    void ReturnSignal(); //返回主界面的信号
};

#endif // CHOOSEGAME_H
