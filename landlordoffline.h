/*****************************
 * 单机斗地主 主游戏
 * **************************/

#ifndef LANDLORDOFFLINE_H
#define LANDLORDOFFLINE_H

#include <QMainWindow>
#include "clock.h"
#include "handcards_landlord.h"
#include "cardpicture.h"
#include "cardgroup.h"
class LandlordOffline : public QMainWindow
{
    Q_OBJECT
    std::vector<CardPicture*> bottomCards; //8张底牌
    std::vector<CardPicture*> cardpictures; // 场上所有牌的图形
public:
    Seat landlord;  //地主
    bool isWin = false; //自己是否赢下了比赛
    bool gameover = false; //是否结束
    HandCardsLandlord* selfCards, *leftCards, *rightCards, *oppositeCards;//手牌
    LandlordOffline(QWidget *parent = nullptr);  //构造函数
    void paintEvent(QPaintEvent* event);
    void StartGame();   //开始游戏，完成发牌操作
    void MainGame();    //发完牌之后开始游戏
    void PaintHandCards(HandCards& handcards); //绘制手牌
    void AddBottomCards(Seat seat); //叫到地主之后，将底牌分配给相应的角色

    CardGroup Last[3]; //上三手出的牌
signals:
    void LandlordSelected();
    void Over();

    void SelfPut(); //自己开始出牌
    void LeftPut();
    void RightPut();
    void OppositePut();
    void PutValid(); //出牌是有效的
public slots:
    void InitGame();  //初始化函数
};

#endif // LANDLORDOFFLINE_H
