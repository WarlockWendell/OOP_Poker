/**********************
 * 手牌区
 *********************/
#ifndef HANDCARDS_H
#define HANDCARDS_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include "cardpicture.h"

class HandCards //: public QWidget
{
    Seat seat;
protected:
    std::vector<CardPicture*> handcards; //牌
public:
    HandCards(Seat seat); //QWidget *parent = nullptr); //构造函数，表明玩家的方位:上家、下家、对家、自家
    void GetOneHand(CardPicture* cardPic); //获取一张手牌
    void SetSeat(Seat s);
    Seat GetSeat();
    void SortCards();  //将牌排序
    std::vector<CardPicture*> GetCards();
signals:

};

#endif // HANDCARDS_H
