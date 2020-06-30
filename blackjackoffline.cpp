#include "blackjackoffline.h"
#include "handcards.h"
#include "card.h"
#include "cardpicture.h"
#include "shuffle.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QPainter>
#include <QEventLoop>
#include <QThread>
#include <ctime>

// 斗地主和 21 点的游戏规则里其实有很多共性的地方，因此其实可以进一步抽象。

Seat BlackjackOffline::lord;    // 游戏的庄家。

BlackjackOffline::BlackjackOffline(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1200,800);
    // 四个玩家的手牌区
    selfCards = new HandCardsBlackjack(Seat::Self);
    leftCards = new HandCardsBlackjack(Seat::Left);
    rightCards = new HandCardsBlackjack(Seat::Right);
    oppositeCards = new HandCardsBlackjack(Seat::Opposite);
}

void BlackjackOffline::InitGame()
{
    qDebug()<<"init game";
    //todo: initialize
}

void BlackjackOffline::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/desk_background.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}

void BlackjackOffline::NewGame()
{
    // 在 21 点的单人游戏中，默认第一局的庄家为玩家，后续则另行定义。
    lord = Self;
    NextGame();
}

void BlackjackOffline::NextGame()
{
    // 初始发牌。在 21 点的游戏中，使用的是去掉了大小王后的52张牌。
    std::vector<int> cardNum = GetCards(52);
    // 生成所有牌的示例。此处参考了斗地主的写法。
    for (int i=0; i<52; i++)
    {
        Card c(cardNum.at(i));
        CardPicture *cardPic = new CardPicture(c,Seat::Central,false,false,this);
        cardPic->setFixedSize(80,105);
        cardPic->setGeometry(this->width()*0.5 - cardPic->width()*0.5,this->height()*0.5-cardPic->width()*0.5,cardPic->width(),cardPic->height());
        cardPic->show();
        cardPic->raise();
        cardpictures.push_back(cardPic);
    }
    // 从庄家右手侧开始，逆时针发牌。
    int StartSeat = (lord + 1) % 4;
    // 发牌代码参考斗地主的写法。21点中，初始每个玩家会被发到两张牌。
    for (int i=7; i>=0; i--)
    {
        Card c(cardNum[i]);
        // 发牌动画
        QPropertyAnimation * Animation = new QPropertyAnimation(cardpictures[i],"geometry");
        Animation->setDuration(50);
        Animation->setStartValue(cardpictures[i]->geometry());
        QEventLoop loop;
        if(StartSeat%4 == Right)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[i]->width()*0.5+500,cardpictures[i]->y(),60,79));
        }
        else if(StartSeat%4 == Opposite)
        {
             Animation->setEndValue(QRect(this->width()*0.5-cardpictures[i]->width()*0.5,cardpictures[i]->y()-250, 60,79));
        }
        else if(StartSeat%4 == Left)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[i]->width()*0.5-500,cardpictures[i]->y(),60,79));
        }
        else if(StartSeat%4 == Self)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[i]->width()*0.5,cardpictures[i]->y() + 280, 80,105));
        }
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        // 延时
        connect(Animation,SIGNAL(finished()),&loop,SLOT(quit()));
        loop.exec();

        // 对于非自己的牌，第一张则背面朝上。
        if (i >= 4 && StartSeat % 4 != Self)
        {
            cardpictures[i]->SetShow(false);
        }
        else
        {
            cardpictures[i]->SetShow(true);
        }

        // 根据不同的位置的牌进行不同绘制。
        switch(StartSeat%4)
        {
            case Seat::Self :
                cardpictures[i]->setFixedSize(80,105);
                cardpictures[i]->SetSeat(Seat::Self);
                cardpictures[i]->raise();
                selfCards->GetOneHand(cardpictures[i]);
            break;
            case Seat::Left:
                cardpictures[i]->SetSeat(Seat::Left);
                cardpictures[i]->raise();
                cardpictures[i]->setFixedSize(60,79);
                leftCards->GetOneHand(cardpictures[i]);
            break;
            case Seat::Right:
                cardpictures[i]->SetSeat(Seat::Right);
                cardpictures[i]->raise();
                cardpictures[i]->setFixedSize(60,79);
                rightCards->GetOneHand(cardpictures[i]);
            break;
            case Seat::Opposite:
                cardpictures[i]->SetSeat(Seat::Opposite);
                cardpictures[i]->raise();
                cardpictures[i]->setFixedSize(60,79);
                oppositeCards->GetOneHand(cardpictures[i]);
            break;
            default:
            break;
        }
        StartSeat ++;
    }
    // 绘制各手牌。
    PaintHandCards(*selfCards);
    PaintHandCards(*leftCards);
    PaintHandCards(*rightCards);
    PaintHandCards(*oppositeCards);
}

void BlackjackOffline::MainGame()
{
    // 主游戏逻辑。
    PaintHandCards(*selfCards);
    Clock *clk = new Clock(30, Self, this); // 每人有 30 秒的思考时间。
    clk->StartClock();
}

//绘制手牌区手牌，也是用了斗地主的代码。
void BlackjackOffline::PaintHandCards(HandCards& handcards)
{
    int size = handcards.GetCards().size();
    Seat seat = handcards.GetSeat();
    switch((int)seat)
    {
        case Seat::Self:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(this->width()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/4,handcards.GetCards().at(i)->y());
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Left:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x() + (i-size/2)*handcards.GetCards().at(i)->width()/4,this->height()*0.5);
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Right:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x() + (i-size/2)*handcards.GetCards().at(i)->width()/4,this->height()*0.5);
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Opposite:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(this->width()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/4,handcards.GetCards().at(i)->y());
                handcards.GetCards().at(i)->raise();
            }
        break;
    }
}
