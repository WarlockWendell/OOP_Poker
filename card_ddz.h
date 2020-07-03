#ifndef CARD_DDZ_H
#define CARD_DDZ_H

#include "card.h"

enum HandType_DDZ
{
    Unknown, //未知
    Pass, //不出
    Single, //单
    SeqSingle, //顺子
    Pair, //对
    SeqPair, //连对
    Triple, //三张
    Plane, //三顺，555666/555666777
    TriplePair, //三带二
    PlanePair, //飞机带翅膀
    Bomb, //炸弹
    BombJoker //王炸
};

//斗地主的单张牌类
class CardDdz : public Card
{
public:
    CardDdz(){}
    CardDdz(int Number):Card(Number){}
    //比较大小，操作符重载
    bool operator > (const CardDdz& card)const;
    bool operator < (const CardDdz& card)const;
    bool operator == (const CardDdz& card)const;
    bool operator != (const CardDdz& card)const;
};

#endif // CARD_DDZ_H
