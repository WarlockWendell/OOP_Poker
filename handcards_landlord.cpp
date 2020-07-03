#include "handcards_landlord.h"

HandCardsLandlord::HandCardsLandlord(Seat s):HandCards(s)
{
}

std::vector<CardPicture*> HandCardsLandlord::PutCard(std::vector<CardDdz> a) //出牌
{
    //将要出的牌从牌组中找出来，从前往后找
    int j = a.size()-1;
    std::vector<CardPicture*> Cards;
    for(int i=(int)handcards.size()-1; i>=0;i--)
    {
        if(handcards[i]->GetCard().GetSuit() == a[j].GetSuit() && handcards[i]->GetCard().GetValue() == a[j].GetValue())
        {
            CardPicture* temp = new CardPicture(handcards[i]->GetCard(),handcards[i]->GetSeat(),true);
            Cards.push_back(temp);
            handcards.at(i)->close();
            handcards.erase(handcards.begin()+i);
            j--;
            if(j<0) //找完了
                break;
        }
    }
    return Cards;
}

//清空要打出去的牌组
void HandCardsLandlord::ClearPutCardList(){
    putCardGroup.setHandType(HandType_DDZ::Unknown);
    putCardGroup.setGroupCount(0);
    putCardGroup.setGroupValue(0);
}
