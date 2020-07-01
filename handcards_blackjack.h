
#ifndef HANDCARDS_BLACKJACK_H
#define HANDCARDS_BLACKJACK_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QString>
#include "handcards.h"
#include "cardpicture.h"

class HandCardsBlackjack : public HandCards
{
    bool isLord = false;    // 是否为庄家
public:
    HandCardsBlackjack(Seat);
    void SetSeat(Seat);
    void GetOneHand(CardPicture *cardPic);
    int GetPoint() const;       // 获取点数
    bool IsHit() const;         // 判断是否牌爆了
    QString GetAttr() const;    // 获取牌型
    bool IsBlackjack() const;   // 是否为黑杰克牌型
    int Compare(const HandCardsBlackjack &Cards) const; // 比较两副牌得分
};

#endif // HANDCARDS_BLACKJACK_H
