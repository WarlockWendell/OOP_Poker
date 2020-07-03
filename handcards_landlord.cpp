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

//计算一组手牌的价值并返回
HandCardsLandlord::HandCardValue HandCardsLandlord::countValue(){
    HandCardValue handcardvalue;

    if(this->HandCardCount==0){ //手牌已经打完
        handcardvalue.sumValue = 0;
        handcardvalue.neededRound = 0;
        return handcardvalue; //返回手牌价值
    }

    HandType_DDZ handtype = this->getHandType(); //获得手牌的类型：未知或者单种牌型
    if(handtype != HandType_DDZ::Unknown){ //如果只剩单种牌型
        handcardvalue.sumValue = this->representPoint(handtype); //牌型的价值
        handcardvalue.neededRound = 1; //只需要一次即可出完
        return handcardvalue; //返回手牌价值
    }

    //无法一次打完
    getPutCardList(); //获得要打出去的牌的最优解，结果放在成员数据中
    CardGroup nowPutCardGroup = this->putCardGroup; //保存当前即将打出的牌
    vector <int> nowPutCardList = this->value_PutCardType;

    for(vector<int>::iterator iter = nowPutCardList.begin();
        iter!=nowPutCardList.end(); iter++){
        this->value_HandCardList[*iter]--;
    }
    this->HandCardCount -= nowPutCardGroup.getGroupCount();
    HandCardValue tem_handcardvalue = countValue(); //递归剩余牌的价值

    for(vector<int>::iterator iter = nowPutCardList.begin();
        iter!=nowPutCardList.end(); iter++){
        this->value_HandCardList[*iter]++;
    }
    this->HandCardCount += nowPutCardGroup.getGroupCount();

    handcardvalue.sumValue = nowPutCardGroup.representPoint(nowPutCardGroup.getHandType())+ tem_handcardvalue.sumValue;
    handcardvalue.neededRound = tem_handcardvalue.neededRound+1;
    return handcardvalue; //返回手牌价值
}

void HandCardsLandlord::getPutCardList(){

}

