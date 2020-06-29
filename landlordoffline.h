/*****************************
 * 单机斗地主 主游戏
 * **************************/

#ifndef LANDLORDOFFLINE_H
#define LANDLORDOFFLINE_H

#include <QMainWindow>
#include "clock.h"
#include "handcards.h"
#include "cardpicture.h"
class LandlordOffline : public QMainWindow
{
    Q_OBJECT
    HandCards* selfCards, *leftCards, *rightCards, *oppositeCards;//手牌
    std::vector<CardPicture*> bottomCards; //8张底牌
    std::vector<CardPicture*> cardpictures; // 场上所有牌的图形
public:
    LandlordOffline(QWidget *parent = nullptr);  //构造函数
    void paintEvent(QPaintEvent* event);
    void StartGame();   //开始游戏，完成发牌操作
    void MainGame();    //发完牌之后开始游戏
    void PaintHandCards(HandCards& handcards); //绘制手牌
    void AddBottomCards(Seat seat); //叫到地主之后，将底牌分配给相应的角色
signals:

public slots:
    void InitGame();  //初始化函数
};

#endif // LANDLORDOFFLINE_H
