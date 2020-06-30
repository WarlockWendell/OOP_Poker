
#ifndef HANDCARDS_BLACKJACK_H
#define HANDCARDS_BLACKJACK_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include "handcards.h"
#include "cardpicture.h"

class HandCardsBlackjack : public HandCards
{
    bool isLord = false;    // 是否为庄家
    int Compare(const HandCardsBlackjack &Cards) const; // 比较两副牌大小
public:
    HandCardsBlackjack(Seat);
    void SetSeat(Seat);
    void GetOneHand(CardPicture *cardPic);
    int GetPoint() const;       // 获取点数
    bool IsHit() const;         // 判断是否牌爆了
    bool IsBlackjack() const;   // 是否为黑杰克牌型
    bool operator< (const HandCardsBlackjack &Cards) const;
    bool operator> (const HandCardsBlackjack &Cards) const;
    bool operator== (const HandCardsBlackjack &Cards) const;
};

#endif // HANDCARDS_BLACKJACK_H
