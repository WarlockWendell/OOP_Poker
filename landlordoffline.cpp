#include "landlordoffline.h"
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
LandlordOffline::LandlordOffline(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1200,800);
    //四个玩家的手牌区
    selfCards = new HandCards(Seat::Self);
    leftCards = new HandCards(Seat::Left);
    rightCards = new HandCards(Seat::Right);
    oppositeCards = new HandCards(Seat::Opposite);
}

void LandlordOffline::InitGame()
{
    qDebug()<<"init game";
    //todo: initialize
}

void LandlordOffline::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/desk_background.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}

void LandlordOffline::StartGame()
{
    std::vector<int> cardNum = GetCards(108);

    //先在屏幕中间显示所有的牌,只画出背面，准备发牌
    for(int i=0; i<108; i++)
    {
        Card c(cardNum.at(i));
        CardPicture* cardPic = new CardPicture(c,Seat::Central,false,this);
        cardPic->setFixedSize(80,105);
        cardPic->setGeometry(this->width()*0.5 - cardPic->width()*0.5,this->height()*0.5-cardPic->width()*0.5,cardPic->width(),cardPic->height());
        cardPic->show();
        cardPic->raise();
        cardpictures.push_back(cardPic);
    }
    //再弄8张牌出来做为第二份底牌，底牌要弄两份，一份用来在屏幕上方显示，另一份给玩家
    for(int i=0; i<8; i++)
    {
        CardPicture* cardPic = new CardPicture(cardpictures[i+100]->GetCard(),Seat::Central,false,this);
        cardPic->setFixedSize(80,105);
        cardPic->setGeometry(this->width()*0.5 - cardPic->width()*0.5,this->height()*0.5-cardPic->width()*0.5,cardPic->width(),cardPic->height());
        cardPic->show();
        cardPic->raise();
        cardpictures.push_back(cardPic);
    }
    //首先将8张底牌显示到正确的位置上
    for(int j = 108; j < 116; j++)
    {
        Card c(cardNum[j]);
        QPropertyAnimation * Animation = new QPropertyAnimation(cardpictures[j],"geometry");
        Animation->setDuration(250-(j-108)*30);
        Animation->setStartValue(cardpictures[j]->geometry());
        QEventLoop loop;
        Animation->setEndValue(QRect(this->width()*0.5+(60*(j-112)),0, 60,79));
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        //延时
        connect(Animation,SIGNAL(finished()),&loop,SLOT(quit()));
        loop.exec();
        cardpictures[j-8]->hide(); //同样的这张牌要藏起来
        cardpictures[j]->SetShow(false);
        cardpictures[j]->setFixedSize(60,79);
        bottomCards.push_back(cardpictures[j]);
    }

    //随机生成一个起始的发牌人
    srand((unsigned int)(time(NULL)));
    int StartSeat = rand() % 4;

    //发牌
    for(int j = 99; j >= 0; j--) //将前面的100张牌发给四个玩家，每个玩家25张牌
    {
        Card c(cardNum[j]);
        QPropertyAnimation * Animation = new QPropertyAnimation(cardpictures[j],"geometry");
        Animation->setDuration(10);
        Animation->setStartValue(cardpictures[j]->geometry());
        QEventLoop loop;
        if(StartSeat%4 == Right)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[j]->width()*0.5+500,cardpictures[j]->y(),80,105));
        }
        else if(StartSeat%4 == Opposite)
        {
             Animation->setEndValue(QRect(this->width()*0.5-cardpictures[j]->width()*0.5,cardpictures[j]->y()-250, 60,79));
        }
        else if(StartSeat%4 == Left)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[j]->width()*0.5-500,cardpictures[j]->y(),80,105));
        }
        else if(StartSeat%4 == Self)
        {
            Animation->setEndValue(QRect(this->width()*0.5-cardpictures[j]->width()*0.5,cardpictures[j]->y() + 280, 80,105));
        }
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        //延时
        connect(Animation,SIGNAL(finished()),&loop,SLOT(quit()));
        loop.exec();

        switch(StartSeat%4)
        {
            case Seat::Self :
                cardpictures[j]->SetShow(true);
                cardpictures[j]->setFixedSize(80,105);
                cardpictures[j]->SetSeat(Seat::Self);
                cardpictures[j]->raise();
                selfCards->GetOneHand(cardpictures[j]);
            break;
            case Seat::Left:
                cardpictures[j]->SetShow(false);
                cardpictures[j]->SetSeat(Seat::Left);
                cardpictures[j]->raise();
                cardpictures[j]->setFixedSize(79,60);
                leftCards->GetOneHand(cardpictures[j]);
            break;
            case Seat::Right:
                cardpictures[j]->SetShow(false);
                cardpictures[j]->SetSeat(Seat::Right);
                cardpictures[j]->raise();
                cardpictures[j]->setFixedSize(79,60);
                rightCards->GetOneHand(cardpictures[j]);
            break;
            case Seat::Opposite:
                cardpictures[j]->SetShow(false);
                cardpictures[j]->SetSeat(Seat::Opposite);
                cardpictures[j]->raise();
                cardpictures[j]->setFixedSize(60,79);
                oppositeCards->GetOneHand(cardpictures[j]);
            break;
        default:
            break;
        }
        StartSeat ++;
    }
    PaintHandCards(*selfCards);
    PaintHandCards(*leftCards);
    PaintHandCards(*rightCards);
    PaintHandCards(*oppositeCards);
    //设置自己的牌可以被点击
    for(int i=0; i<(int)selfCards->GetCards().size(); i++)
    {
        selfCards->GetCards().at(i)->SetAllowClick(true);
    }
}

//主游戏逻辑
void LandlordOffline::MainGame()
{
    AddBottomCards(Self);
    PaintHandCards(*selfCards);
    Clock* clk = new Clock(10,Self,this);
    clk->StartClock();
    //clk->show();
}

//绘制手牌区手牌
void LandlordOffline::PaintHandCards(HandCards& handcards)
{
    int size = handcards.GetCards().size();
    Seat seat = handcards.GetSeat();
    handcards.SortCards();
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
            if(handcards.GetCards().at(0)->GetShow() == false)  //如果是正常情况
                for(int i=0; i<size; i++)
                {
                    handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x(),this->height()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/5);
                    handcards.GetCards().at(i)->raise();
                }
            else  //牌显示，也就是明牌
                for(int i=0; i<size; i++)
                {
                    handcards.GetCards().at(i)->setFixedSize(70,90);
                    handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x()+(i%5-2)*handcards.GetCards().at(i)->width()/4 ,this->height()*0.5 + (i/5-(size/5+1)/2)*handcards.GetCards().at(i)->height()/2);
                    handcards.GetCards().at(i)->raise();
                }
        break;
        case Seat::Right:
            if(handcards.GetCards().at(0)->GetShow() == false)  //如果是正常情况
                for(int i=0; i<size; i++)
                {
                    handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x(),this->height()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/5);
                    handcards.GetCards().at(i)->raise();
                }
            else  //牌显示，也就是明牌
                for(int i=0; i<size; i++)
                {
                    handcards.GetCards().at(i)->setFixedSize(70,90);
                    handcards.GetCards().at(i)->move(handcards.GetCards().at(i)->x()+(i%5-1)*handcards.GetCards().at(i)->width()/4 ,this->height()*0.5 + (i/5-(size/5+1)/2)*handcards.GetCards().at(i)->height()/2);
                    handcards.GetCards().at(i)->raise();
                }
            break;
        break;
        case Seat::Opposite:
        if(handcards.GetCards().at(0)->GetShow() == false)  //如果是正常情况
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(this->width()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/4,handcards.GetCards().at(i)->y());
                handcards.GetCards().at(i)->raise();
            }
        else  //牌显示，也就是明牌
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->move(this->width()*0.5 + (i-size/2-2)*handcards.GetCards().at(i)->width()/3,handcards.GetCards().at(i)->y());
                handcards.GetCards().at(i)->setFixedSize(70,90);
                handcards.GetCards().at(i)->raise();
            }
        break;
    }
}

//加底牌
void LandlordOffline::AddBottomCards(Seat seat) //这个人当地主，给它底牌
{
    switch((int)seat)
    {
        case Seat::Self:
            for(int i = 0; i<(int)bottomCards.size(); i++)
            {
                cardpictures[i+100]->setFixedSize(selfCards->GetCards().at(i)->size());
                cardpictures[i+100]->SetSeat(Seat::Self);
                cardpictures[i+100]->SetShow(true);
                cardpictures[i+100]->move(selfCards->GetCards().at(i)->x(),selfCards->GetCards().at(0)->y());
                selfCards->GetOneHand(cardpictures[i+100]);
                cardpictures[i+100]->show();
            }
            for(int i=100; i<108; i++)
            {
                cardpictures[i]->SetAllowClick(true);
            }
        break;
        case Seat::Left:
            for(int i = 0; i<(int)bottomCards.size(); i++)
            {
                cardpictures[i+100]->setFixedSize(leftCards->GetCards().at(i)->size());
                cardpictures[i+100]->SetSeat(Seat::Left);
                cardpictures[i+100]->SetShow(leftCards->GetCards().at(0)->GetShow());
                cardpictures[i+100]->move(leftCards->GetCards().at(i)->x(),leftCards->GetCards().at(0)->y());
                leftCards->GetOneHand(cardpictures[i+100]);
                cardpictures[i+100]->show();
            }
        break;
        case Seat::Right:
            for(int i = 0; i<(int)bottomCards.size(); i++)
            {
                cardpictures[i+100]->setFixedSize(rightCards->GetCards().at(i)->size());
                cardpictures[i+100]->SetSeat(Seat::Right);
                cardpictures[i+100]->SetShow(rightCards->GetCards().at(0)->GetShow());
                cardpictures[i+100]->move(rightCards->GetCards().at(i)->x(),rightCards->GetCards().at(0)->y());
                rightCards->GetOneHand(cardpictures[i+100]);
                cardpictures[i+100]->show();
            }
        break;
        case Seat::Opposite:
            for(int i = 0; i<(int)bottomCards.size(); i++)
            {
                cardpictures[i+100]->setFixedSize(oppositeCards->GetCards().at(i)->size());
                cardpictures[i+100]->SetSeat(Seat::Opposite);
                cardpictures[i+100]->SetShow(oppositeCards->GetCards().at(0)->GetShow());
                cardpictures[i+100]->move(oppositeCards->GetCards().at(i)->x(),oppositeCards->GetCards().at(0)->y());
                oppositeCards->GetOneHand(cardpictures[i+100]);
                cardpictures[i+100]->show();
            }
        break;
    }
    for(int i=0; i<(int)bottomCards.size();i++)
    {
        bottomCards[i]->SetShow(true);
    }
}
