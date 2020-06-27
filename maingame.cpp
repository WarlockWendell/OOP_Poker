#include "maingame.h"
#include "handcards.h"
#include "card.h"
#include "cardpicture.h"
#include "shuffle.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QPainter>
#include <QEventLoop>
#include <QThread>
MainGame::MainGame(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1200,800);

}

void MainGame::InitGame()
{
    qDebug()<<"init game";
    //todo: initialize
}
void MainGame::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/desk_background.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}
void MainGame::StartGame()
{
    std::vector<int> cardNum = GetCards(108);
    std::vector<CardPicture*> cardpictures;
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
    //qDebug()<<cardpictures.size();
    //发牌
    int StartSeat = 0;  //todo :随机生成一个起始的发牌人

    int len = cardpictures.size();
    for(int j = len - 1; j >= 0; j--)
    {
        QPropertyAnimation * Animation = new QPropertyAnimation(cardpictures[j],"geometry");
        Animation->setDuration(150);
        Animation->setStartValue(cardpictures[j]->geometry());
        QEventLoop loop;
        //todo：对8张底牌的处理
        if(StartSeat%4 == Right)
        {

            Animation->setEndValue(QRect(cardpictures[j]->x()+400,cardpictures[j]->y(),80,105));
        }
        else if(StartSeat%4 == Opposite)
        {
             Animation->setEndValue(QRect(cardpictures[j]->x(),cardpictures[j]->y() - 250, 80,105));
        }
        else if(StartSeat%4 == Left)
        {

            Animation->setEndValue(QRect(cardpictures[j]->x()-400,cardpictures[j]->y(),80,105));
        }
        else if(StartSeat%4 == Self)
        {
            Animation->setEndValue(QRect(cardpictures[j]->x(),cardpictures[j]->y() + 250, 80,105));
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
            break;
            case Seat::Left:
                cardpictures[j]->SetSeat(Seat::Left);
                cardpictures[j]->update();
                cardpictures[j]->raise();
                cardpictures[j]->SetShow(false);
                cardpictures[j]->setFixedSize(79,60);
            break;
            case Seat::Right:
                cardpictures[j]->SetSeat(Seat::Right);
                cardpictures[j]->update();
                cardpictures[j]->raise();
                cardpictures[j]->SetShow(false);
                cardpictures[j]->setFixedSize(79,60);
            break;
            case Seat::Opposite:
                cardpictures[j]->SetSeat(Seat::Opposite);
                cardpictures[j]->update();
                cardpictures[j]->raise();
                cardpictures[j]->SetShow(false);
                cardpictures[j]->setFixedSize(60,79);
            break;
        default:
            break;
        }
        StartSeat ++;
        //todo:将牌展开、排序
    }










}



