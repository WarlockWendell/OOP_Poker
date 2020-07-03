#include "putcards.h"
#include <QEventLoop>
static void Switch(LandlordOffline& l,Seat seat)
{
    switch ((int) seat)
    {
        case Seat::Self:
        {
            PutSelf(l);
            break;
        }
        case Seat::Left:
        {
            PutLeft(l);
            break;
        }
        case Seat::Right:
        {
            PutRight(l);
            break;
        }
        case Seat::Opposite:
        {
            PutOpposite(l);
            break;
        }
    }
}


void PutCards(LandlordOffline& l)
{
    Switch(l,l.landlord); //第一次出牌

    QEventLoop loop;
    QObject::connect(&l,&LandlordOffline::Over,&loop,&QEventLoop::quit);
    loop.exec();
}

void PutSelf(LandlordOffline& l)
{
    emit l.SelfPut();
    if(l.gameover == true)
        return;
    Clock* clk = new Clock (20,Self,&l); //时钟

    //出牌按钮
    MyPushButton* Put = new MyPushButton(":/Image/button.png");
    Put->setParent(&l);
    Put->setFixedSize(140,46);
    QLabel* PutText = new QLabel(Put);
    PutText->setFixedSize(140,46);
    PutText->setAlignment(Qt::AlignCenter);
    PutText->setText("出牌");
    PutText->setStyleSheet("color:green");
    PutText->setFont(QFont("黑体",20,QFont::Light));
    Put->move((&l)->width()/2 - 220, (&l)->height()/2 + 150);

    //不出的按钮
    MyPushButton* NotPut = new MyPushButton(":/Image/button.png");
    NotPut->setParent(&l);
    NotPut->setFixedSize(140,43);
    QLabel* NotPutText = new QLabel(NotPut);
    NotPutText->setFixedSize(140,46);
    NotPutText->setAlignment(Qt::AlignCenter);
    NotPutText->setText("不出");
    NotPutText->setStyleSheet("color:green");
    NotPutText->setFont(QFont("黑体",20,QFont::Light));
    NotPut->move((&l)->width()/2 + 70, (&l)->height()/2 + 150);

    Put->show();
    if(l.Last[0].getHandType() != Pass || l.Last[1].getHandType() != Pass || l.Last[1].getHandType() != Pass) //三个人中有人出了牌你才能不出
        NotPut->show();

    clk->StartClock();  //开始计时

    QObject::connect(Put,&MyPushButton::MousePress,[&]()  //出牌
    {
         CardGroup a;
         a.copyGroup(GetSelectedCards(l));
         //判断出牌是否有效
         if((a.getHandType() != Unknown && a.compareTo(l.Last[0]) == CardGroup::Larger && \
            a.compareTo(l.Last[1]) == CardGroup::Larger && a.compareTo(l.Last[2]) == CardGroup::Larger&& a.getHandType()!=Pass)) //有效
         {
             std::vector<CardPicture*> cards;
             cards = l.selfCards->PutCard(GetSelectedCards(l));
             l.Last[2] = l.Last[1];
             l.Last[1] = l.Last[0];
             l.Last[0] = a;
             emit l.PutValid();
             if(l.selfCards->GetCards().size() == 0) //出完了
             {
                l.isWin = true;
                l.gameover = true;
                emit l.Over();
             }
             l.PaintHandCards(*l.selfCards);
             for(int i=0; i<(int)cards.size();i++)
             {
                 int size = cards.size();
                 cards[i]->setFixedSize(80,105);
                 cards[i]->move(l.width()*0.5-cards[i]->width()*0.5,l.height()/2 + 100);
                 cards[i]->move(l.width()*0.5 + (i-size/2-2)*cards[i]->width()/4,cards[i]->y());
                 cards[i]->setParent(&l);
                 cards[i]->show();
                 QObject::connect(&l,&LandlordOffline::SelfPut,cards[i],&CardPicture::close);
             }
             PutRight(l);
         }
    });

    QObject::connect(NotPut,&MyPushButton::MousePress,[&](){
            CardGroup a;
            l.Last[2] = l.Last[1];
            l.Last[1] = l.Last[0];
            l.Last[0] = a;
            QLabel* notPutLabel = new QLabel(&l);
            notPutLabel->setText("不出");
            notPutLabel->setStyleSheet("color:yellow");
            notPutLabel->setFont(QFont("黑体",28,QFont::Light));
            notPutLabel->move(l.width()/2-notPutLabel->width()/2,l.height()/2+150);
            notPutLabel->show();
            QObject::connect(&l,&LandlordOffline::SelfPut,notPutLabel,&QLabel::close);

            PutRight(l);
    });

    QObject::connect(clk,&Clock::TimeOut,[&](){
        //前面有人出了牌，超时就是不出
        if(l.Last[0].getHandType() != Pass || l.Last[1].getHandType() != Pass || l.Last[1].getHandType() != Pass)
        {
            CardGroup a;
            l.Last[2] = l.Last[1];
            l.Last[1] = l.Last[0];
            l.Last[0] = a;
            QLabel* notPutLabel = new QLabel(&l);
            notPutLabel->setText("不出");
            notPutLabel->setStyleSheet("color:yellow");
            notPutLabel->setFont(QFont("黑体",28,QFont::Light));
            notPutLabel->move(l.width()/2-notPutLabel->width()/2,l.height()/2+150);
            notPutLabel->show();
            QObject::connect(&l,&LandlordOffline::SelfPut,notPutLabel,&QLabel::close);
            PutRight(l);
        }
        else //前面没有人出牌，超时就出最小的一张牌
        {
            CardGroup a;
            std::vector<CardDdz> temp;
            Card card = l.selfCards->GetCards().at(l.selfCards->GetCards().size() - 1)->GetCard();
            CardDdz mincard;
            mincard.SetValue(card.GetValue());
            mincard.SetSuit(card.GetSuit());
            temp.push_back(mincard);
            a.copyGroup(temp);

            std::vector<CardPicture*> cards;
            cards = l.selfCards->PutCard(temp);
            l.Last[2] = l.Last[1];
            l.Last[1] = l.Last[0];
            l.Last[0] = a;
            emit l.PutValid();

            for(int i=0; i<(int)cards.size();i++)
            {
                int size = cards.size();
                cards[i]->setFixedSize(80,105);
                cards[i]->move(l.width()*0.5-cards[i]->width()*0.5,l.height()/2 + 100);
                cards[i]->move(l.width()*0.5 + (i-size/2-2)*cards[i]->width()/4,cards[i]->y());
                cards[i]->setParent(&l);
                cards[i]->show();
                QObject::connect(&l,&LandlordOffline::SelfPut,cards[i],&CardPicture::close);
            }
            if(l.selfCards->GetCards().size() == 0) //出完了
            {
               l.isWin = true;
               l.gameover = true;
               emit l.Over();
            }
            l.PaintHandCards(*l.selfCards);
            PutRight(l);
        }
    });
    QObject::connect(NotPut,&MyPushButton::MousePress,Put,&MyPushButton::close);
    QObject::connect(NotPut,&MyPushButton::MousePress,NotPut,&MyPushButton::close);
    QObject::connect(&l,&LandlordOffline::PutValid,Put,&MyPushButton::close);
    QObject::connect(&l,&LandlordOffline::PutValid,NotPut,&MyPushButton::close);
    QObject::connect(clk,&Clock::TimeOut,Put,&MyPushButton::close);
    QObject::connect(clk,&Clock::TimeOut,NotPut,&MyPushButton::close);
    QObject::connect(&l,&LandlordOffline::PutValid,clk,&Clock::StopClock);
    QObject::connect(NotPut,&MyPushButton::MousePress,clk,&Clock::StopClock);
}


void PutLeft(LandlordOffline& l)
{
    emit l.LeftPut();
    if(l.gameover == true)
        return;
    Clock* clk = new Clock (5,Left,&l); //时钟
    clk->StartClock();

    //如果不出
    QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
    {
        if(l.Last[0].getHandType() != Pass || l.Last[1].getHandType() != Pass || l.Last[2].getHandType() != Pass)
        {
             CardGroup a;
             l.Last[2] = l.Last[1];
             l.Last[1] = l.Last[0];
             l.Last[0] = a;
             QLabel* notPutLabel = new QLabel(&l);
             notPutLabel->setText("不出");
             notPutLabel->setStyleSheet("color:yellow");
             notPutLabel->setFont(QFont("黑体",28,QFont::Light));
             notPutLabel->move(l.width()/2-notPutLabel->width()/2 - 250,l.height()/2);
             notPutLabel->show();
             QObject::connect(&l,&LandlordOffline::LeftPut,notPutLabel,&QLabel::close);
             PutSelf(l);
        }
        else
        {
             CardGroup a;
             std::vector<CardDdz> temp;
             Card card = l.leftCards->GetCards().at(l.leftCards->GetCards().size() - 1)->GetCard();
             CardDdz mincard;
             mincard.SetValue(card.GetValue());
             mincard.SetSuit(card.GetSuit());
             temp.push_back(mincard);
             a.copyGroup(temp);

             std::vector<CardPicture*> cards;
             cards = l.leftCards->PutCard(temp);
             l.Last[2] = l.Last[1];
             l.Last[1] = l.Last[0];
             l.Last[0] = a;
             emit l.PutValid();

             for(int i=0; i<(int)cards.size();i++)
             {
                 int size = cards.size();
                 cards[i]->setFixedSize(60,79);
                 cards[i]->move(l.width()*0.5-cards[i]->width()*0.5-400,l.height()/2 - cards[i]->height()/2);
                 cards[i]->move(cards[i]->x(),l.height()*0.5 + (i-size/2-2)*cards[i]->width()/5);
                 cards[i]->setParent(&l);
                 cards[i]->show();
                 QObject::connect(&l,&LandlordOffline::LeftPut,cards[i],&CardPicture::close);
             }
             if(l.leftCards->GetCards().size() == 0) //出完了
             {
                l.isWin = true;
                if(l.landlord != Seat::Left)
                    l.gameover = true;
                emit l.Over();
             }
             l.PaintHandCards(*l.leftCards);
             PutSelf(l);
        }
    });
}

void PutRight(LandlordOffline& l)
{
    emit l.RightPut();
    if(l.gameover == true)
        return;
    Clock* clk = new Clock (5,Right,&l); //时钟
    clk->StartClock();

    QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
    {
        if(l.Last[0].getHandType() != Pass || l.Last[1].getHandType() != Pass || l.Last[2].getHandType() != Pass)
        {
            CardGroup a;
            l.Last[2] = l.Last[1];
            l.Last[1] = l.Last[0];
            l.Last[0] = a;
            QLabel* notPutLabel = new QLabel(&l);
            notPutLabel->setText("不出");
            notPutLabel->setStyleSheet("color:yellow");
            notPutLabel->setFont(QFont("黑体",28,QFont::Light));
            notPutLabel->move(l.width()/2-notPutLabel->width()/2+250,l.height()/2);
            notPutLabel->show();
            QObject::connect(&l,&LandlordOffline::RightPut,notPutLabel,&QLabel::close);
            PutOpposite(l);
        }
         else
         {
              CardGroup a;
              std::vector<CardDdz> temp;
              Card card = l.rightCards->GetCards().at(l.rightCards->GetCards().size() - 1)->GetCard();
              CardDdz mincard;
              mincard.SetValue(card.GetValue());
              mincard.SetSuit(card.GetSuit());
              temp.push_back(mincard);
              a.copyGroup(temp);

              std::vector<CardPicture*> cards;
              cards = l.rightCards->PutCard(temp);
              l.Last[2] = l.Last[1];
              l.Last[1] = l.Last[0];
              l.Last[0] = a;
              emit l.PutValid();

              for(int i=0; i<(int)cards.size();i++)
              {
                  int size = cards.size();
                  cards[i]->setFixedSize(60,79);
                  cards[i]->move(l.width()*0.5-cards[i]->width()*0.5+400,l.height()/2-cards[i]->height()/2);
                  cards[i]->move(cards[i]->x(),l.height()*0.5 + (i-size/2-2)*cards[i]->width()/5);
                  cards[i]->setParent(&l);
                  cards[i]->show();
                  QObject::connect(&l,&LandlordOffline::RightPut,cards[i],&CardPicture::close);
              }
              if(l.rightCards->GetCards().size() == 0) //出完了
              {
                 l.isWin = true;
                 if(l.landlord != Seat::Right)
                    l.gameover = true;
                 emit l.Over();
              }
              l.PaintHandCards(*l.rightCards);
              PutOpposite(l);
         }
    });
}

void PutOpposite(LandlordOffline& l)
{
    emit l.OppositePut();
    if(l.gameover == true)
        return;
    Clock* clk = new Clock (5,Opposite,&l); //时钟
    clk->StartClock();
    QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
    {
        if(l.Last[0].getHandType() != Pass || l.Last[1].getHandType() != Pass || l.Last[2].getHandType() != Pass)
        {
            CardGroup a;
            l.Last[2] = l.Last[1];
            l.Last[1] = l.Last[0];
            l.Last[0] = a;
            QLabel* notPutLabel = new QLabel(&l);
            notPutLabel->setText("不出");
            notPutLabel->setStyleSheet("color:yellow");
            notPutLabel->setFont(QFont("黑体",28,QFont::Light));
            notPutLabel->move(l.width()/2-notPutLabel->width()/2,l.height()/2-150);
            notPutLabel->show();
            QObject::connect(&l,&LandlordOffline::OppositePut,notPutLabel,&QLabel::close);
            PutLeft(l);
        }
        else
     {
         CardGroup a;
         std::vector<CardDdz> temp;
         Card card = l.oppositeCards->GetCards().at(l.oppositeCards->GetCards().size() - 1)->GetCard();
         CardDdz mincard;
         mincard.SetValue(card.GetValue());
         mincard.SetSuit(card.GetSuit());
         temp.push_back(mincard);
         a.copyGroup(temp);

         std::vector<CardPicture*> cards;
         cards = l.oppositeCards->PutCard(temp);
         l.Last[2] = l.Last[1];
         l.Last[1] = l.Last[0];
         l.Last[0] = a;
         emit l.PutValid();

         for(int i=0; i<(int)cards.size();i++)
         {
             int size = cards.size();
             cards[i]->setFixedSize(60,79);
             cards[i]->move(l.width()*0.5-cards[i]->width()*0.5,cards[i]->y()+200);
             cards[i]->move(l.width()*0.5 + (i-size/2-2)*cards[i]->width()/4,cards[i]->y());
             cards[i]->setParent(&l);
             cards[i]->show();
             QObject::connect(&l,&LandlordOffline::OppositePut,cards[i],&CardPicture::close);
         }
         if(l.oppositeCards->GetCards().size() == 0) //出完了
         {
            l.isWin = true;
            if(l.landlord != Seat::Opposite)
                l.gameover = true;
            emit l.Over();
         }
         l.PaintHandCards(*l.oppositeCards);
         PutLeft(l);
        }
    });
}

//获得想要出的牌
std::vector<CardDdz> GetSelectedCards(LandlordOffline& l)
{
    std::vector<Card> SelectedIndex; //暂存选中的牌
    for(int i=0; i<(int)l.selfCards->GetCards().size(); i++)
    {
       if(l.selfCards->GetCards().at(i)->GetSelete() == true)
       {
           SelectedIndex.push_back(l.selfCards->GetCards().at(i)->GetCard()); //将已经选择的牌全部选出来
       }
    }
    std::vector<CardDdz> cards;
    for(int i=0; i<(int)SelectedIndex.size();i++)
    {
        CardDdz card;
        card.SetValue(SelectedIndex[i].GetValue());
        card.SetSuit(SelectedIndex[i].GetSuit());
        cards.push_back(card);
    }
    return cards;
}


