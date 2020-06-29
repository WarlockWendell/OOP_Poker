//构建一张扑克牌的值和花色

#include "card.h"

//按照牌值和花色构造一张扑克牌
Card::Card(CardValue v, CardSuit s) : Value(v), Suit(s)
{
}

//按照序号构造
Card::Card(int Number)
{
    //第一副牌1-54， 第二副牌55 - 108
    if(Number >= 55 && Number <= 108)
        Number -= 54; //转换为1 - 54

     if(Number >= 1 && Number <= 52) //非大小王
     {
        Value = (CardValue)(( Number - 1 ) % 13 + 1);  //牌值,范围是1-13
        Suit = (CardSuit)(( Number - 1) / 13 + 1); //花色，1方片，2梅花，3红桃，4黑桃
     }
     else if( Number == 53 || Number == 54)
     {
         Value = CardValue(Number);  //小大王牌值为53 、54
         Suit = Suit_Begin;  //大小王的花色设置为1
     }
}

//拷贝构造函数
Card::Card(const Card& c) : Value(c.Value),Suit(c.Suit)
{
}

//设置牌值，应该用不到
void Card::SetValue(const CardValue &v)
{
    Value = v;
}
//设置花色，应该也用不到
void Card::SetSuit(const CardSuit &s)
{
    Suit = s;
}

//取牌值
CardValue Card::GetValue()const
{
    return Value;
}

//取花色
CardSuit Card::GetSuit()const
{
    return Suit;
}
