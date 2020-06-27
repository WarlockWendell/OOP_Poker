#include "handcards.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QRect>
HandCards::HandCards(Seat s):seat(s)
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
void HandCards::GetOneHand(CardPicture* cardPic)
{
    handcards.push_back(cardPic);
}

//降序排序
static bool GreatSort(CardPicture* a, CardPicture* b)
{
    int valuea = (int)a->GetCard().GetValue();
    int valueb = (int)b->GetCard().GetValue();
    if(valuea == 1)
        valuea = 14;
    else if(valuea == 2)
        valuea = 15;
    if(valueb == 1)
        valueb = 14;
    else if(valueb == 2)
        valueb = 15;

    if(valuea > valueb)
        return true;
    else
        return false;
}
void HandCards::SortCards()
{
    std::sort(handcards.begin(),handcards.end(),GreatSort);
}

std::vector<CardPicture*> HandCards::GetCards()
{
    return handcards;
}

