#ifndef HANDCARDSLANDLORD_H
#define HANDCARDSLANDLORD_H
#include "cardpicture.h"
#include "handcards.h"
#include "card_ddz.h"
#include "cardgroup.h"
#include <vector>
#include <QDebug>


class HandCardsLandlord : public HandCards
{
    bool isWin; //是否获胜，即出完了
    struct HandCardValue{ //一副手牌的价值
        int sumValue; //总价值
        int neededRound; //需要打几手牌
    };
public:
    int value_HandCardList[56] = {0}; //记录每种类型的牌有多少张
    CardGroup putCardGroup; //要打出去的牌型
    vector <int> value_PutCardList; //记录要打出去的牌

    HandCardsLandlord(Seat);
    void ClearPutCardList(); //在出牌前，先清空要打出去的牌组
    void GetValueHandCardList(); //获取手牌中每种牌有多少张的信息，存放在value_HandCardList中
    void InitHandCard(); //初始化手牌
    HandCardValue countValue(); //计算一组手牌的价值并返回
    void GetPutCardList_1(); //获得主动打出去的牌型的最优解
public slots:
    std::vector<CardPicture*> PutCard(std::vector<CardDdz>); //出牌

};





#endif // HANDCARDSLANDLORD_H

