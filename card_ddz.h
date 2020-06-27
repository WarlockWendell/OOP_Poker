/************************
 * 斗地主纸牌结构定义
 * *********************/
#ifndef CARD_DDZ_H
#define CARD_DDZ_H

#include"card.h"

//斗地主出牌类型
enum HandType_DDZ
{
    Hand_Unknown, //未知
    Hand_Pass,  //要不起
    Hand_Single, //单张
    Hand_Pair, //一对
    Hand_Triple, //三个
    Hand_Triple_Single, //三带一，根据“三”进行比较
    Hand_Triple_Pair, //三带二
    Hand_Plane, //飞机，至少两个连续大小的三张
    Hand_Plane_Single, //飞机带单
    Hand_Plane_Pair, //飞机带对子
    Hand_Seq_Single, //顺子，不少于5张，34567
    Hand_Seq_Pair, //连对，不少于3对，不能带2和怪
    Hand_Bomb, //炸弹
    Hand_Bomb_Jokers //王炸
};

//斗地主的单张牌类
class CardDdz : public Card
{
    CardDdz(CardValue v = Card_Begin, CardSuit s = Suit_Begin):Card(v,s){}
    CardDdz(int Number):Card(Number){}
    //比较大小，操作符重载
    bool operator > (const CardDdz& card)const;
    bool operator < (const CardDdz& card)const;
    bool operator == (const CardDdz& card)const;
    bool operator != (const CardDdz& card)const;
};

#endif // CARD_DDZ_H
