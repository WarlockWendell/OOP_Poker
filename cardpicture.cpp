#include "cardpicture.h"
#include <QDebug>
#include <QPainter>
#include <QMatrix>

//构造函数，构造牌的一些基本信息，显示与否，显示的角度，生成牌的正反面图片

CardPicture::CardPicture(const Card& c ,Seat se, bool isshow,QWidget *parent) : QWidget(parent),card(c),seat(se),IsShow(isshow)
{
    QPixmap CardPicAll(":/Image/card.png"); //所有的扑克牌的图片
    int v = (int)card.GetValue();
    int s = (int)card.GetSuit();
    if(v>=1&&v<=13)
        CardPicFront= CardPicAll.copy((v-1)*80,(4-s)*105,80,105); //取出对应的一张牌
    else if(v == 53)
           CardPicFront = CardPicAll.copy(0,420,80,105);  //小王
    else if(v == 54)
           CardPicFront = CardPicAll.copy(80,420,80,105); //大王
    CardPicBack = CardPicAll.copy(160,420,80,105); //背面
}

CardPicture::CardPicture(const Card& c, Seat se, bool isshow, bool needrotate, QWidget* parent) : QWidget(parent),card(c),seat(se),NeedRotate(needrotate), IsShow(isshow)
{
    QPixmap CardPicAll(":/Image/card.png"); //所有的扑克牌的图片
    int v = (int)card.GetValue();
    int s = (int)card.GetSuit();
    if(v>=1&&v<=13)
        CardPicFront= CardPicAll.copy((v-1)*80,(4-s)*105,80,105); //取出对应的一张牌
    else if(v == 53)
           CardPicFront = CardPicAll.copy(0,420,80,105);  //小王
    else if(v == 54)
           CardPicFront = CardPicAll.copy(80,420,80,105); //大王
    CardPicBack = CardPicAll.copy(160,420,80,105); //背面
}
CardPicture::CardPicture(const Card& c, Seat se, bool isshow, bool needrotate, int type, QWidget* parent) : QWidget(parent),card(c),seat(se),NeedRotate(needrotate),type(type), IsShow(isshow)
{
    QPixmap CardPicAll(":/Image/card.png"); //所有的扑克牌的图片
    int v = (int)card.GetValue();
    int s = (int)card.GetSuit();
    if(v>=1&&v<=13)
        CardPicFront= CardPicAll.copy((v-1)*80,(4-s)*105,80,105); //取出对应的一张牌
    else if(v == 53)
           CardPicFront = CardPicAll.copy(0,420,80,105);  //小王
    else if(v == 54)
           CardPicFront = CardPicAll.copy(80,420,80,105); //大王
    if(type==1)
    {
      CardPicBack = CardPicAll.copy(160,420,80,105); //背面
    }
    else
    {
      CardPicBack.load(":/Image/bei.png");
    }
}

void CardPicture::SetCard(Card c)
{
    card = c;
}

void CardPicture::SetShow(bool s)
{
    IsShow = s;
    update();
}

void CardPicture::SetSeat(Seat s)
{
    seat = s;
    //update();
}
Card CardPicture::GetCard()
{
    return card;
}
Seat CardPicture::GetSeat()
{
    return seat;
}
bool CardPicture::GetShow()
{
    return IsShow;
    update();
}

QPixmap CardPicture::GetCardPicBack()
{
    return CardPicBack;
}
QPixmap CardPicture::GetCardPicFront()
{
    return CardPicFront;
}
void CardPicture::SetSelected(bool b)
{
    IsSelected = b;
}

void CardPicture::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    if(IsShow)
    {
        painter.drawPixmap(this->rect(),CardPicFront);
    }
    else
    {
        if(seat != Self && seat != Central && NeedRotate) //不是自家，需要旋转；如果牌是显示状态的，比如明牌、斗地主的底牌等都不需要进行旋转，是背面时如果是别家的就需要旋转
        {
            //qDebug()<<seat;
            QMatrix matrix ; //旋转矩阵
            double angle;
            switch(seat)
            {
                case Self:
                    angle = 0.0;
                break;
                case Left:
                    angle = 90.0;
                break;
                case Opposite:
                    angle = 180.0;
                break;
                case Right:
                    angle = 270.0;
                break;
            default:
                angle = 0.0;
            }

            matrix.rotate((angle)); //旋转
            if(IsRotate == false)
            {
                CardPicBack = CardPicBack.transformed(matrix);
                IsRotate = true;
            }

        }
        painter.drawPixmap(this->rect(),CardPicBack);
    }

}

void CardPicture::SetAllowClick(bool b)
{
    AllowClick = b;
}


void CardPicture::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(AllowClick == false)
        return;
    if(event->button()&Qt::LeftButton)
    {
        if(seat == Seat::Self)  //只有自己的牌才能被自己的鼠标选择
        {
            if(IsSelected == false)
            {
                IsSelected = true;
                QRect rect = this->geometry();
                rect.setY(rect.y()-15);
                this->setGeometry(rect);
            }
            else
            {
                IsSelected = false;
                QRect rect = this->geometry();
                rect.setY(rect.y()+15);
                this->setGeometry(rect);
            }
        }
    }
}
