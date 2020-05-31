/*********************************
 * 画一张牌的类
 * ******************************/
#ifndef CARDPICTURE_H
#define CARDPICTURE_H

#include <QWidget>
#include "card.h"

enum Seat{
    Self = 0,
    Left = 1 ,//上家
    Opposite = 2, //对家
    Right = 3, //下家
    Central = 4   //中心
};

class CardPicture : public QWidget
{
    Q_OBJECT
    Card card;  //牌
    Seat seat = Self; //表示上家、下家与对家
    bool IsShow = true; //是否展示牌，选否表示展示背面
    QPixmap CardPicFront; //牌的正面
    QPixmap CardPicBack;  //牌的反面
public:
    CardPicture(const Card& c, Seat s, bool isshow, QWidget* parent = nullptr);
    void SetCard(Card c);
    void SetSeat(Seat seat);
    void SetShow(bool s);
    Card GetCard();
    Seat GetSeat();
    bool GetShow();
    QPixmap GetCardPicFront();
    QPixmap GetCardPicBack();
protected:
    void paintEvent(QPaintEvent *event);

signals:
};

#endif // CARDPICTURE_H
