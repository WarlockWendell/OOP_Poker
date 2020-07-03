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
public:
    HandCardsLandlord(Seat);

public slots:
    std::vector<CardPicture*> PutCard(std::vector<CardDdz>); //出牌

};





#endif // HANDCARDSLANDLORD_H
