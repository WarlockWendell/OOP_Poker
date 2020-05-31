#include "handcards.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QRect>
HandCards::HandCards(Seat s, QWidget *parent):QWidget(parent),seat(s)
{
}
void HandCards::SetSeat(Seat s)
{
    seat = s;
}

Seat HandCards::GetSeat()
{
    return seat;
}

//发一张牌，参数是发的牌
void HandCards::GetOneHand(Card &card)
{
    handcards.push_back(card);  //添加一张牌
}

std::vector<Card> HandCards::GetCard()
{
    return handcards;
}


