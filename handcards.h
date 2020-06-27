/**********************
 * 手牌区
 *********************/
#ifndef HANDCARDS_H
#define HANDCARDS_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include "cardpicture.h"
class HandCards : public QWidget
{
    Q_OBJECT
    Seat seat;
    std::vector<Card> handcards;  //牌
public:
    HandCards(Seat seat, QWidget *parent = nullptr); //构造函数，表明玩家的方位:上家、下家、对家、自家
    void GetOneHand(Card& card); //获取一张手牌
    void SetSeat(Seat s);
    Seat GetSeat();
    std::vector<Card> GetCard();
signals:

};

#endif // HANDCARDS_H
