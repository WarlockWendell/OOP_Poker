#include "handcards_blackjack.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QDebug>
#include <QRect>

HandCardsBlackjack::HandCardsBlackjack(Seat s) : HandCards(s) {}

void HandCardsBlackjack::SetSeat(Seat s)
{
    HandCards::SetSeat(s);
}

void HandCardsBlackjack::GetOneHand(CardPicture *cardPic)
{
    HandCards::GetOneHand(cardPic);
}

int HandCardsBlackjack::GetPoint() const
{
    // 本函数计算一副手牌的点数固定部分。
    int point = 0;
    int pointvar = 0; // 点数的可变部分
    for (CardPicture *handcard : handcards)
    {
        // 对每一副牌进行遍历。
        const int value = handcard->GetCard().GetValue();
        if (value >= 1 && value <= 10)
        {
            // 数字牌的点数为其牌面数值。
            point += value;
            if (value == 1)
            {
                // A 同时可以当做 1 和 11.
                pointvar += 10;
            }
        }
        else if (value >= 11 && value <= 13)
        {
            // JQK 的点数为 10.
            point += 10;
        }
    }
    while (point <= 11 && pointvar >= 10)
    {
        // 最后为可以取的点数中最接近 21 且不超过的值。
        // P.S. 实际游戏中最多只可能加一次。
        point += 10;
        pointvar -= 10;
    }
    return point;
}

bool HandCardsBlackjack::IsHit() const
{
    return GetPoint() > 21;
}

bool HandCardsBlackjack::IsBlackjack() const
{
    // 黑杰克判定：一张牌为 A，一张点数为 10.
    if (handcards.size() != 2)
    {
        // 不是 2 张牌，一定不是黑杰克
        return false;
    }
    bool hasA = false, has10 = false;
    for (CardPicture *handcard : handcards)
    {
        const int value = handcard->GetCard().GetValue();
        if (value == 1)
        {
            // A。
            if (hasA)
            {
                // 两张 A，一定不是黑杰克
                return false;
            }
            else
            {
                hasA = true;
            }
        }
        else if (value >= 11 && value <= 13)
        {
            // 10。
            if (has10)
            {
                // 两张 10，一定不是黑杰克
                return false;
            }
            else
            {
                has10 = true;
            }
        }
        else
        {
            // 有别的牌，一定不是黑杰克
            return false;
        }
    }
    return true;
}

QString HandCardsBlackjack::GetAttr() const
{
    QString result;
    if (this->IsHit())
    {
        result.append("爆牌");
    }
    else if (this->IsBlackjack())
    {
        result.append("黑杰克！");
    }
    else if (!this->IsHit() && handcards.size() >= 5)
    {
        result.append("五小！");
    }
    else
    {
        QString point = QString::number(this->GetPoint());
        result.append(point);
        result.append(" 点");
    }
    return result;
}

int HandCardsBlackjack::Compare (const HandCardsBlackjack &Cards) const
{
    // 比较两副手牌哪一个更“大”，从而得到得分。
    // 21 点游戏基于手牌的“大小”进行胜负的判断。
    int result = 0;
    if (this->IsBlackjack())
    {
        // 为黑杰克的情况
        if (Cards.IsBlackjack())
        {
            // 同为黑杰克，平局
            result = 0;
        }
        else
        {
            // 有黑杰克的胜利。黑杰克得分为1.5倍。
            result = 3;
        }
    }
    else if (this->IsHit())
    {
        // 爆牌的情况
        if (Cards.IsHit())
        {
            // 同样是爆牌时，庄家获胜。且一定是闲家与庄家比较。
            result = -2;
        }
        else
        {
            // 否则，不爆牌的一方获胜
            result = -2;
        }
    }
    else if (Cards.IsHit())
    {
        // 对面爆牌的情况，此时这边没有爆牌，必赢。
        result = 2;
    }
    else if (Cards.IsBlackjack())
    {
        // 对面为黑杰克的情况，此时这边不是黑杰克，必输。
        result = -3;
    }
    else if (!this->IsHit() && handcards.size() >= 5)
    {
        // 五张未爆的牌(“五小”)比上述情况外的任意牌点数都大。
        if (!Cards.IsHit() && Cards.handcards.size() >= 5)
        {
            // 都为“五小”，平局。
            result = 0;
        }
        else
        {
            result = 2;
        }
    }
    else if (!Cards.IsHit() && Cards.handcards.size() >= 5)
    {
        // 对面“五小”，必输。
        result = -2;
    }
    else
    {
        // 剩余情况下比点数。
        if (this->GetPoint() < Cards.GetPoint())
        {
            // 自己这边小，输了。
            result = -2;
        }
        else if (this->GetPoint() > Cards.GetPoint())
        {
            // 自己这边大，赢了。
            result = 2;
        }
        else
        {
            // 点数相同时，比较牌的张数。牌多的赢。
            if (this->handcards.size() < Cards.handcards.size())
            {
                // 自己牌较少，因此输了。
                result = -2;
            }
            else if (this->handcards.size() > Cards.handcards.size())
            {
                // 自己牌多，赢了
                result = 2;
            }
            else
            {
                // 点数和牌数相同，平局。
                result = 0;
            }
        }
    }
    return result;
}
/*
bool HandCardsBlackjack::operator< (const HandCardsBlackjack &Cards) const
{
    return (this->Compare(Cards) < 0);
}

bool HandCardsBlackjack::operator> (const HandCardsBlackjack &Cards) const
{
    return (this->Compare(Cards) > 0);
}

bool HandCardsBlackjack::operator== (const HandCardsBlackjack &Cards) const
{
    return (this->Compare(Cards) == 0);
}*/
