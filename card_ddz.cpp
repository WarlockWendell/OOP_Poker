#include "card_ddz.h"

//比较牌的点数,操作符重载>
bool CardDdz::operator > (const CardDdz &card) const
{
    return Value>card.Value;
}

//比较牌的点数,操作符重载<
bool CardDdz::operator < (const CardDdz &card) const
{
    return Value < card.Value;
}

//比较牌的点数,操作符重载==
bool CardDdz::operator == (const CardDdz &card) const
{
    return Value == card.Value;
}

//比较牌的点数,操作符重载!=
bool CardDdz::operator != (const CardDdz &card) const
{
    return Value != card.Value;
}
