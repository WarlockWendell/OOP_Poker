#include "handcards.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QRect>
HandCards::HandCards(Seat s):seat(s)
{
}

//设置玩家的位置
void HandCards::SetSeat(Seat s)
{
    seat = s;
}

//获得玩家的位置信息
Seat HandCards::GetSeat()
{
    return seat;
}

//为成员数据“手牌张数”赋值
void HandCards::SetCount(unsigned int c){
    this->HandCardCount = c;
}

//获取手牌的张数
unsigned int HandCards::GetCount()const{
    return handcards.size();
}

//发一张牌，参数是发的牌
void HandCards::GetOneHand(CardPicture* cardPic)
{
    handcards.push_back(cardPic);
    //update ： 由于HandCards继承cardGroup，所以在增加一张牌时需要也添到m_cardset中
    CardDdz card;
    card.SetSuit(cardPic->GetCard().GetSuit());
    card.SetValue(cardPic->GetCard().GetValue());
    addOne(card);
}

//降序排序,比较两张牌的大小，若a>b则返回true
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
static bool GreaterSort(CardDdz a, CardDdz b)
{
    int valuea = (int)a.GetValue();
    int valueb = (int)b.GetValue();
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
    //update:需要将m_cardset中的牌也按同样的方式排序
    std::sort(m_cardset.begin(),m_cardset.end(),GreaterSort);
}

//获得手牌堆
std::vector<CardPicture*> HandCards::GetCards()
{
    return handcards;
}
