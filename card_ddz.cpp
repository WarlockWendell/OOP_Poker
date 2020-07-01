#include "card_ddz.h"

//比较牌的点数,操作符重载>
bool CardDdz::operator > (const CardDdz &card) const
{
    int valuethis = this->GetValue();
    int valuecard = card.GetValue();
    if(valuethis == 1 || valuethis == 2)
        valuethis += 13;
    else if (valuecard == 1 || valuecard == 2)
        valuecard += 13;
    return valuethis > valuecard;
}

//比较牌的点数,操作符重载<
bool CardDdz::operator < (const CardDdz &card) const
{
    int valuethis = this->GetValue();
    int valuecard = card.GetValue();
    if(valuethis == 1 || valuethis == 2)
        valuethis += 13;
    else if (valuecard == 1 || valuecard == 2)
        valuecard += 13;
    return valuethis < valuecard;
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

