#ifndef CARD_DDZ_H
#define CARD_DDZ_H

#include "card.h"

enum HandType_DDZ
{
    Hand_Unknown, //未知
    Hand_Pass, //不出
    Hand_Single, //单
    Hand_Seq_Single, //顺子
    Hand_Pair, //对
    Hand_Seq_Pair, //连对
    Hnad_Triple, //三张
    Hand_Plane, //三顺，555666/555666777
    Hand_Triple_Pair, //三带二
    Hand_Plane_Pair, //飞机带翅膀
    Hand_Bomb, //炸弹
    Hand_Bomb_Joker //王炸
};

//斗地主的单张牌类
class CardDdz : public Card
{
public:
    CardDdz(int Number):Card(Number){}
    //比较大小，操作符重载
    bool operator > (const CardDdz& card)const;
    bool operator < (const CardDdz& card)const;
    bool operator == (const CardDdz& card)const;
    bool operator != (const CardDdz& card)const;
};

#endif // CARD_DDZ_H
