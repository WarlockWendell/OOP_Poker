/*********************************
 * 画一张牌的类
 * ******************************/
#ifndef CARDPICTURE_H
#define CARDPICTURE_H

#include <QWidget>
#include <QMouseEvent>
#include "card.h"
#include <QGraphicsScene>
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
    int type = 1;//牌的类型，21点用的牌卡背和斗地主不一样，21点用的type=2
    Card card;  //牌
    Seat seat = Self; //表示上家、下家与对家
    bool NeedRotate = true; // 是否需要旋转(21点的牌均不需旋转)
    bool IsShow = true; //是否展示牌，选否表示展示背面
    bool IsRotate = false; //是否已经旋转过了
    bool AllowClick = false; //这张牌是否可以点击
    bool IsSelected = false; //这张牌是否被选择了
    QPixmap CardPicFront; //牌的正面
    QPixmap CardPicBack;  //牌的反面
public:
    CardPicture(const Card& c, Seat s, bool isshow, QWidget* parent = nullptr);
    CardPicture(const Card& c, Seat s, bool isshow, bool needrotate, QWidget* parent = nullptr);
    CardPicture(const Card& c, Seat s, bool isshow, bool needrotate, int type, QWidget* parent = nullptr);//在21点游戏里，传入type=2定制了不一样的卡背
    void SetCard(Card c);
    void SetSeat(Seat seat);
    void SetShow(bool s);
    void SetSelected(bool b);
    Card GetCard();
    Seat GetSeat();
    bool GetShow();
    void SetAllowClick(bool b);
    QPixmap GetCardPicFront();
    QPixmap GetCardPicBack();
    void mousePressEvent(QMouseEvent *event);
protected:
    void paintEvent(QPaintEvent *event);

signals:
    void Seletected();
};

#endif // CARDPICTURE_H
