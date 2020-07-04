#include "handcards_landlord.h"
#include <string>

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
            //update : 由于HandCards继承自CardGroup, 所以减少一张牌时，也需要减少m_cardset中的牌
            m_cardset.erase(m_cardset.begin()+i);
            j--;
            if(j<0) //找完了
                break;
        }
    }
    return Cards;
}

//清空要打出去的牌组
void HandCardsLandlord::ClearPutCardList(){
    value_PutCardList.clear(); //清空出牌序列表
    putCardGroup.setHandType(HandType_DDZ::Unknown); //初始化"待出牌组"的牌型为未知
    putCardGroup.setGroupCount(0); //初始化"待出牌组"的张数为0
    putCardGroup.setGroupValue(0); //初始化"待出牌组"的价值为0
}

//获取手牌中每种牌有多少张的信息，存放在value_HandCardList中
void HandCardsLandlord::GetValueHandCardList(){
    memset(value_HandCardList, 0, sizeof(value_HandCardList));
    for(vector<CardPicture*>::iterator iter = handcards.begin();
        iter!=handcards.end(); iter++){
        value_HandCardList[(*iter)->GetCard().GetValue()]++; //根据牌的点数记录每种牌的张数
    }
}

//初始化手牌
void HandCardsLandlord::InitHandCard(){
    SetCount(handcards.size()); //初始化手牌张数
    SortCards(); //降序排序
    GetValueHandCardList(); //初始化每种牌的张数信息
}

//计算一组手牌的价值并返回
HandCardsLandlord::HandCardValue HandCardsLandlord::countValue(){
    HandCardValue handcardvalue;

    if(this->GetCount()==0){ //手牌已经打完
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
    GetPutCardList_1(); //获得要打出去的牌的最优解，结果放在成员数据中
    CardGroup nowPutCardGroup = this->putCardGroup; //保存当前即将打出的牌
    vector <int> nowPutCardList = this->value_PutCardList;

    for(vector<int>::iterator iter = nowPutCardList.begin();
        iter!=nowPutCardList.end(); iter++){
        this->value_HandCardList[*iter]--;
    }
    unsigned int handcardcount = this->GetCount() - nowPutCardGroup.getGroupCount();
    this->SetCount(handcardcount);
    HandCardValue tem_handcardvalue = countValue(); //递归剩余牌的价值

    for(vector<int>::iterator iter = nowPutCardList.begin();
        iter!=nowPutCardList.end(); iter++){
        this->value_HandCardList[*iter]++;
    }
    handcardcount = this->GetCount() + nowPutCardGroup.getGroupCount();
    this->SetCount(handcardcount);

    handcardvalue.sumValue = nowPutCardGroup.representPoint(nowPutCardGroup.getHandType())+ tem_handcardvalue.sumValue;
    handcardvalue.neededRound = tem_handcardvalue.neededRound+1;
    return handcardvalue; //返回手牌价值
}

//获得要打出去的牌型的最优解
void HandCardsLandlord::GetPutCardList_1(){
    this->ClearPutCardList(); //清空出牌区
    //如果只剩最后一手牌
    HandType_DDZ handtype = this->getHandType(); //获得手牌的类型：未知或者单种牌型
    if(handtype!=HandType_DDZ::Unknown){ //不是非法牌型

    }
    //只剩王炸(四张天王牌)和另一手牌
    if(this->value_PutCardList[53]+this->value_PutCardList[54]==4){

    }
    //保存最佳的价值
    HandCardValue BestHandCardValue;
    BestHandCardValue.sumValue = 0;
    BestHandCardValue.neededRound = 20;
    BestHandCardValue.neededRound+=1;

    //保存最佳组合
    CardGroup BestCardGroup;

    //带出去的牌
    int nMax; //起决定作用的牌
    int tmp1 = 0;

    //优先出三带二和飞机牌型
    for(int i=1;i<14;i++){ //遍历A-K
        if(this->value_HandCardList[i] % 4 != 0){
            //出三带二
            if(this->value_HandCardList[i] >2){
                for(int j=1;j<55;j++){
                    this->value_HandCardList[i]-=3;
                    if(this->value_HandCardList[j]>1){
                        this->value_HandCardList[j]-=2;
                        this->SetCount(this->GetCount()-5);
                        HandCardValue tmpHandCardValue = countValue();
                        this->value_HandCardList[j]+=2;
                        this->SetCount(this->GetCount()+5);
                        if((BestHandCardValue.sumValue*2-(BestHandCardValue.neededRound)) <=
                                tmpHandCardValue.sumValue*2 - (tmpHandCardValue.neededRound)){
                            BestHandCardValue = tmpHandCardValue;
                            BestCardGroup.setHandType(HandType_DDZ::TriplePair);
                            BestCardGroup.setGroupCount(5);
                            BestCardGroup.addOne(CardDdz(i));BestCardGroup.addOne(CardDdz(i));
                            BestCardGroup.addOne(CardDdz(i));
                            BestCardGroup.addOne(CardDdz(j));BestCardGroup.addOne(CardDdz(j));
                            tmp1 = j;
                            nMax = i;
                        }
                    }
                    this->value_HandCardList[i]-=3;
                }
            }
        }
    }
    //将主动出牌时的最佳牌型存至成员数据putCardGroup中
    if(BestCardGroup.getHandType() == HandType_DDZ::TriplePair){
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(tmp1);
        this->value_PutCardList.push_back(tmp1);
        this->putCardGroup = BestCardGroup;
        return;
    }
    else if(BestCardGroup.getHandType()==HandType_DDZ::PlanePair){
    //飞机待完善
    }
    //再处理价值最低的牌
    for(int i=1;i<14;i++){
        //出单牌
        if(this->value_HandCardList[i]>0){
            this->value_HandCardList[i]--;
            this->SetCount(this->GetCount()-1);
            HandCardValue tmpHandCardValue = countValue();
            this->value_HandCardList[i]++;
            this->SetCount(GetCount()+1);
            if((BestHandCardValue.sumValue*2-BestHandCardValue.neededRound) <=
                    tmpHandCardValue.sumValue*2-tmpHandCardValue.neededRound){
                BestHandCardValue = tmpHandCardValue;
                BestCardGroup.setHandType(HandType_DDZ::Single);
                BestCardGroup.setGroupCount(1);
                BestCardGroup.addOne(CardDdz(i));
                nMax = i;
            }
        }
        //出对牌
        if(this->value_PutCardList[i]>1){
            this->value_HandCardList[i]-=2;
            this->SetCount(GetCount()-2);
            HandCardValue tmpHandCardValue = countValue();
            this->value_HandCardList[i]+=2;
            this->SetCount(GetCount()+2);
            if((BestHandCardValue.sumValue*2-BestHandCardValue.neededRound) <=
                    tmpHandCardValue.sumValue*2-tmpHandCardValue.neededRound){
                BestHandCardValue = tmpHandCardValue;
                BestCardGroup.setHandType(HandType_DDZ::Pair);
                BestCardGroup.setGroupCount(2);
                BestCardGroup.addOne(CardDdz(i));BestCardGroup.addOne(CardDdz(i));
                nMax = i;
            }
        }
        //出三牌
        if(this->value_PutCardList[i]>2){
            this->value_HandCardList[i]-=3;
            this->SetCount(GetCount()-3);
            HandCardValue tmpHandCardValue = countValue();
            this->value_HandCardList[i]+=3;
            this->SetCount(GetCount()+3);
            if((BestHandCardValue.sumValue*2-BestHandCardValue.neededRound) <=
                    tmpHandCardValue.sumValue*2-tmpHandCardValue.neededRound){
                BestHandCardValue = tmpHandCardValue;
                BestCardGroup.setHandType(HandType_DDZ::Triple);
                BestCardGroup.setGroupCount(3);
                BestCardGroup.addOne(CardDdz(i));BestCardGroup.addOne(CardDdz(i));
                BestCardGroup.addOne(CardDdz(i));
                nMax = i;
            }
        }
    }
    //将主动出牌时的最佳牌型存至成员数据putCardGroup中
    if(BestCardGroup.getHandType() == HandType_DDZ::Single){
        this->value_PutCardList.push_back(nMax);
        this->putCardGroup = BestCardGroup;
        return;
    }
    else if(BestCardGroup.getHandType() == HandType_DDZ::Pair){
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(nMax);
        this->putCardGroup = BestCardGroup;
        return;
    }
    else if(BestCardGroup.getHandType() == HandType_DDZ::Triple){
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(nMax);
        this->value_PutCardList.push_back(nMax);
        this->putCardGroup = BestCardGroup;
        return;
    }
    this->putCardGroup.setHandType(HandType_DDZ::Unknown); //返回异常
}


