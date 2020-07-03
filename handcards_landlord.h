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
    int HandCardCount; //手牌张数
    vector<CardDdz> handCardsLandlord; //牌
    struct HandCardValue{ //一副手牌的价值
        int sumValue; //总价值
        int neededRound; //需要打几手牌
    };
public:
    int value_HandCardList[18] = {0}; //记录每种类型的牌有多少张
    vector <int> value_PutCardType; //记录要打出去的牌
    HandCardsLandlord(Seat);
    HandCardValue countValue(); //计算一组手牌的价值并返回
public:
    CardGroup putCardGroup; //要打出去的牌型
    void ClearPutCardList(); //清空要打出去的牌组
    void getPutCardList(); //获得要打出去的牌型
public slots:
    std::vector<CardPicture*> PutCard(std::vector<CardDdz>); //出牌

};





#endif // HANDCARDSLANDLORD_H

