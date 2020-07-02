#include "blackjackoffline.h"
#include "handcards.h"
#include "card.h"
#include "cardpicture.h"
#include "shuffle.h"
#include "mypushbutton.h"
#include <vector>
#include <QDebug>
#include <QPropertyAnimation>
#include <QPainter>
#include <QEventLoop>
#include <QTimer>
#include <QString>

// 斗地主和 21 点的游戏规则里其实有很多共性的地方，因此其实可以进一步抽象。

BlackjackOffline::BlackjackOffline(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1200,800);
}

void BlackjackOffline::InitGame()
{
    // 这个是从斗地主那复制粘贴过来的，似乎是多余的，但是不敢删_(:3JL)_
}

void BlackjackOffline::paintEvent(QPaintEvent *event)
{
    // 绘制游戏背景
    QWidget::paintEvent(event);
    QPixmap pix;
    QPixmap toDraw;
    pix.load(":/Image/blackjack_table.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
    // 绘制玩家图标
    // 自己
    QString fname = ":/Image/";
    if (lord != Seat::Self)
    {
        fname.append("non");
    }
    fname.append("lord_icon.png");
    pix.load(fname);
    if (player != Seat::Self)
    {
        toDraw = pix.copy(0, 0, 32, 32);
    }
    else
    {
        toDraw = pix.copy(32, 0, 32, 32);
    }
    painter.drawPixmap(this->width()/2-250, this->height()/2+230, 32, 32, toDraw);
    // 下家
    fname = ":/Image/";
    if (lord != Seat::Left)
    {
        fname.append("non");
    }
    fname.append("lord_icon.png");
    pix.load(fname);
    if (player != Seat::Left)
    {
        toDraw = pix.copy(0, 0, 32, 32);
    }
    else
    {
        toDraw = pix.copy(32, 0, 32, 32);
    }
    painter.drawPixmap(this->width()/2-550, this->height()/2+50, 32, 32, toDraw);
    // 上家
    fname = ":/Image/";
    if (lord != Seat::Right)
    {
        fname.append("non");
    }
    fname.append("lord_icon.png");
    pix.load(fname);
    if (player != Seat::Right)
    {
        toDraw = pix.copy(0, 0, 32, 32);
    }
    else
    {
        toDraw = pix.copy(32, 0, 32, 32);
    }
    painter.drawPixmap(this->width()/2+450, this->height()/2+50, 32, 32, toDraw);
    // 对家
    fname = ":/Image/";
    if (lord != Seat::Opposite)
    {
        fname.append("non");
    }
    fname.append("lord_icon.png");
    pix.load(fname);
    if (player != Seat::Opposite)
    {
        toDraw = pix.copy(0, 0, 32, 32);
    }
    else
    {
        toDraw = pix.copy(32, 0, 32, 32);
    }
    painter.drawPixmap(this->width()/2-100, this->height()/2-320, 32, 32, toDraw);
    // 绘制玩家文本
    painter.setPen(QColor(255, 255, 255));
    QFont font;
    font.setPointSize(16);
    painter.setFont(font);
    QTextOption option(Qt::AlignLeft | Qt::AlignTop);
    option.setWrapMode(QTextOption::WordWrap);
    painter.drawText(QRect(this->width()/2-250, this->height()/2+270, this->width(), this->height()), selfLabel, option);
    painter.drawText(QRect(this->width()/2-550, this->height()/2+90, this->width(), this->height()), leftLabel, option);
    painter.drawText(QRect(this->width()/2+450, this->height()/2+90, this->width(), this->height()), rightLabel, option);
    painter.drawText(QRect(this->width()/2-60, this->height()/2-310, this->width(), this->height()), oppositeLabel, option);
}

void BlackjackOffline::NewGame()
{
    // 在 21 点的单人游戏中，默认第一局的庄家为玩家，后续则另行定义。
    lord = Self;
    StartGame();
}

void BlackjackOffline::ExitGame()
{
    // 进行内存清空工作
    delete selfCards;
    delete leftCards;
    delete rightCards;
    delete oppositeCards;
    // 删除所有牌
    for (CardPicture *card : cardpictures)
    {
        delete card;
    }
    for (CardPicture *card : usedDeck)
    {
        delete card;
    }
    cardpictures.clear();
    usedDeck.clear();
}

void BlackjackOffline::NextGame()
{
    ExitGame();
    StartGame();
}

void BlackjackOffline::StartGame()
{
    // 四个玩家的手牌区
    selfCards = new HandCardsBlackjack(Seat::Self);
    leftCards = new HandCardsBlackjack(Seat::Left);
    rightCards = new HandCardsBlackjack(Seat::Right);
    oppositeCards = new HandCardsBlackjack(Seat::Opposite);
    // 初始发牌。在 21 点的游戏中，使用的是去掉了大小王后的52张牌。
    std::vector<int> cardNum = GetCards(52);
    // 生成所有牌的示例。此处参考了斗地主的写法。
    for (int i=0; i<52; i++)
    {
        Card c(cardNum.at(i));
        CardPicture *cardPic = new CardPicture(c,Seat::Central,false,false,2,this);
        cardPic->setFixedSize(80,105);
        cardPic->setGeometry(this->width()*0.5 - cardPic->width()*0.5,this->height()*0.5-cardPic->width()*0.5,cardPic->width(),cardPic->height());
        cardPic->show();
        cardPic->raise();
        cardpictures.push_back(cardPic);
    }
    // 从庄家下家开始，依次发牌。
    int StartSeat = (lord + 1) % 4;
    // 发牌代码部分参考斗地主的写法。21点中，初始每个玩家会被发到两张牌。
    for (int i=0; i<8; i++)
    {
        // 发牌动画
        CardPicture *c = cardpictures.at(cardpictures.size()-1);
        cardpictures.pop_back();
        usedDeck.push_back(c);
        QPropertyAnimation * Animation = new QPropertyAnimation(c,"geometry");
        Animation->setDuration(50);
        Animation->setStartValue(c->geometry());
        QEventLoop loop;
        if(StartSeat%4 == Right)
        {
            Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5+500,c->y(),60,79));
        }
        else if(StartSeat%4 == Opposite)
        {
             Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5,c->y()-250, 60,79));
        }
        else if(StartSeat%4 == Left)
        {
            Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5-500,c->y(),60,79));
        }
        else if(StartSeat%4 == Self)
        {
            Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5,c->y() + 280, 80,105));
        }
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        // 延时
        connect(Animation,SIGNAL(finished()),&loop,SLOT(quit()));
        loop.exec();

        // 对于非自己的庄家牌，第一张则背面朝上。
        if (i < 4 && StartSeat % 4 != Self && StartSeat % 4 == lord)
        {
            c->SetShow(false);
        }
        else
        {
            c->SetShow(true);
        }

        // 根据不同的位置的牌进行不同绘制。
        switch(StartSeat%4)
        {
            case Seat::Self :
                c->setFixedSize(80,105);
                c->SetSeat(Seat::Self);
                c->raise();
                selfCards->GetOneHand(c);
            break;
            case Seat::Left:
                c->SetSeat(Seat::Left);
                c->raise();
                c->setFixedSize(60,79);
                leftCards->GetOneHand(c);
            break;
            case Seat::Right:
                c->SetSeat(Seat::Right);
                c->raise();
                c->setFixedSize(60,79);
                rightCards->GetOneHand(c);
            break;
            case Seat::Opposite:
                c->SetSeat(Seat::Opposite);
                c->raise();
                c->setFixedSize(60,79);
                oppositeCards->GetOneHand(c);
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

    // 确定庄家手牌。
    HandCardsBlackjack *lordcard;
    if (lord == Seat::Left)
    {
        lordcard = leftCards;
    }
    else if (lord == Seat::Right)
    {
        lordcard = rightCards;
    }
    else if (lord == Seat::Opposite)
    {
        lordcard = oppositeCards;
    }
    else
    {
        lordcard = selfCards;
    }
    // 显示各个玩家分数。
    QString score;
    selfLabel = "总分：";
    score = QString::number(selfScore);
    selfLabel.append(score);
    leftLabel = "总分：";
    score = QString::number(leftScore);
    leftLabel.append(score);
    rightLabel = "总分：";
    score = QString::number(rightScore);
    rightLabel.append(score);
    oppositeLabel = "总分：";
    score = QString::number(oppositeScore);
    oppositeLabel.append(score);
    // 进行出牌
    PaintHandCards(*selfCards);
    player = lord % 4;    // 当前出牌的玩家。由庄家开始出牌。
    for (int i = 0; i < 4; i++)
    {
        this->update();
        // 4个玩家分别选择抽牌和停牌。
        if (player != Seat::Self)
        {
            HandCardsBlackjack *card;    // 当前玩家的手牌
            if (player == Seat::Left)
            {
                card = leftCards;
            }
            else if (player == Seat::Right)
            {
                card = rightCards;
            }
            else
            {
                card = oppositeCards;
            }
            // 其他玩家进行游戏，自动出牌。
            // 这里出牌的逻辑是，当小于 16 点时必定抓牌，16-21 点则停牌概率为 83%, 67%, 50%, ...
            int chance = 6;
            while (chance > 0)
            {
                // 庄家点数大于等于 17 时必须停牌。
                int point = card->GetPoint();
                if (point >= 17 && player == lord)
                {
                    break;
                }
                if (point < 16)
                {
                    chance = 6;
                }
                else if (point <= 21)
                {
                    chance = 21 - point;
                }
                else
                {
                    chance = 0;
                    break;
                }
                // 人为创造一定延时
                QEventLoop loop1;
                QTimer::singleShot(450, &loop1, SLOT(quit()));
                loop1.exec();
                // 随机取牌
                srand((unsigned int)(time(NULL)));
                int roll = rand() % 6;  // 随机
                if (roll > chance)
                {
                    break;  // 停牌
                }
                else
                {
                    // 抽一张牌
                    CardPicture *c = cardpictures.at(cardpictures.size()-1);
                    cardpictures.pop_back();
                    usedDeck.push_back(c);
                    // 发牌动画
                    QPropertyAnimation * Animation = new QPropertyAnimation(c,"geometry");
                    Animation->setDuration(50);
                    Animation->setStartValue(c->geometry());
                    QEventLoop loop2;
                    if (player == Right)
                    {
                        Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5+500,this->height()*0.5,60,79));
                    }
                    else if (player == Opposite)
                    {
                         Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5,this->height()*0.5-250, 60,79));
                    }
                    else if (player == Left)
                    {
                        Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5-500,this->height()*0.5,60,79));
                    }
                    Animation->start(QAbstractAnimation::DeleteWhenStopped);
                    connect(Animation,SIGNAL(finished()),&loop2,SLOT(quit()));
                    loop2.exec();
                    // 新发的牌正面朝上
                    c->SetShow(true);
                    // 根据不同的位置的牌进行不同绘制。
                    switch (player)
                    {
                        case Seat::Left:
                            c->SetSeat(Seat::Left);
                            c->raise();
                            c->setFixedSize(60,79);
                            leftCards->GetOneHand(c);
                        break;
                        case Seat::Right:
                            c->SetSeat(Seat::Right);
                            c->raise();
                            c->setFixedSize(60,79);
                            rightCards->GetOneHand(c);
                        break;
                        case Seat::Opposite:
                            c->SetSeat(Seat::Opposite);
                            c->raise();
                            c->setFixedSize(60,79);
                            oppositeCards->GetOneHand(c);
                        break;
                        default:
                        break;
                    }
                }
                PaintHandCards(*card);
            }
            QEventLoop loop3;
            QTimer::singleShot(250, &loop3, SLOT(quit()));
            loop3.exec();
        }
        else
        {
            // 玩家的回合此时玩家可以选择抽牌或者停牌。
            // 开始计时。
            Clock *clk = new Clock(30, Self, this); // 每人有 30 秒的思考时间。若超时则直接停牌。
            MyPushButton* DrawButton = new MyPushButton(":/Image/draw_a_card_button.png");
            MyPushButton* HaltButton = new MyPushButton(":/Image/halt_button.png");
            MyPushButton* DisabledButton = new MyPushButton(":/Image/draw_a_card_button_disabled.png");
            DisabledButton->setParent(this);
            DisabledButton->hide();
            DisabledButton->move(this->width()*0.5 - 70 - DrawButton->width()/2, this->height() - 50);
            DrawButton->setParent(this);
            HaltButton->setParent(this);
            DrawButton->show();
            HaltButton->show();
            DrawButton->move(this->width()*0.5 - 70 - DrawButton->width()/2, this->height() - 50);
            HaltButton->move(this->width()*0.5 + 70 - HaltButton->width()/2, this->height() - 50);
            clk->StartClock();
            QEventLoop loop4;
            connect(DrawButton, &MyPushButton::MouseRelease, [=](){
                // 自己抽一张牌
                CardPicture *c = cardpictures.at(cardpictures.size()-1);
                cardpictures.pop_back();
                usedDeck.push_back(c);
                // 发牌的动画
                QPropertyAnimation * Animation = new QPropertyAnimation(c,"geometry");
                Animation->setDuration(50);
                Animation->setStartValue(c->geometry());
                QEventLoop loop5;
                Animation->setEndValue(QRect(this->width()*0.5-c->width()*0.5,c->y() + 280, 80,105));
                Animation->start(QAbstractAnimation::DeleteWhenStopped);
                // 延时
                connect(Animation,SIGNAL(finished()),&loop5,SLOT(quit()));
                loop5.exec();
                c->SetShow(true);
                c->setFixedSize(80,105);
                c->SetSeat(Seat::Self);
                c->raise();
                selfCards->GetOneHand(c);
                PaintHandCards(*selfCards);
                if (selfCards->GetPoint() > 21)
                {
                    // 如果爆牌则无法继续抽牌。
                    DisabledButton->AlterPicture(":/Image/card_hit_button.png");
                    DrawButton->hide();
                    DisabledButton->show();
                }
                else if (selfCards->GetPoint() >= 17 && player == lord)
                {
                    // 庄家在点数超过17时无法抽牌。
                    DrawButton->hide();
                    DisabledButton->show();
                }
            });
            connect(HaltButton, &MyPushButton::MouseRelease, &loop4, &QEventLoop::quit);
            connect(clk, &Clock::TimeOut, &loop4, &QEventLoop::quit);
            loop4.exec();
            delete clk;
            delete DrawButton;
            delete HaltButton;
            delete DisabledButton;
        }
        player = (player + 1) % 4;
    }
    player = -1;
    this->update();
    // 揭晓庄家的第一张手牌。
    lordcard->GetCards().at(0)->SetShow(true);
    // 休息0.25s，开始计分
    QEventLoop loop6;
    QTimer::singleShot(250, &loop6, SLOT(quit()));
    loop6.exec();
    int score1 = 0, score2 = 0, score3 = 0, score4 = 0; // 自己、下家、上家、对家的本局得分。
    if (lord != Self)
    {
        score1 = selfCards->Compare(*lordcard);
    }
    if (lord != Left)
    {
        score2 = leftCards->Compare(*lordcard);
    }
    if (lord != Right)
    {
        score3 = rightCards->Compare(*lordcard);
    }
    if (lord != Opposite)
    {
        score4 = oppositeCards->Compare(*lordcard);
    }
    if (lord == Self)
    {
        score1 = 0 - (score2 + score3 + score4);
    }
    if (lord == Left)
    {
        score2 = 0 - (score1 + score3 + score4);
    }
    if (lord == Right)
    {
        score3 = 0 - (score1 + score2 + score4);
    }
    if (lord == Opposite)
    {
        score4 = 0 - (score1 + score2 + score3);
    }
    // 绘制本局得分。
    selfLabel.append("\r\n");
    selfLabel.append(selfCards->GetAttr());
    selfLabel.append("\r\n本局：");
    score = QString::number(score1);
    selfLabel.append(score);
    leftLabel.append("\r\n");
    leftLabel.append(leftCards->GetAttr());
    leftLabel.append("\r\n本局：");
    score = QString::number(score2);
    leftLabel.append(score);
    rightLabel.append("\r\n");
    rightLabel.append(rightCards->GetAttr());
    rightLabel.append("\r\n本局：");
    score = QString::number(score3);
    rightLabel.append(score);
    oppositeLabel.append(" ");
    oppositeLabel.append(oppositeCards->GetAttr());
    oppositeLabel.append(" 本局：");
    score = QString::number(score4);
    oppositeLabel.append(score);
    this->update();
    // 将分数加到总分
    selfScore += score1;
    leftScore += score2;
    rightScore += score3;
    oppositeScore += score4;
    QEventLoop loop7;
    QTimer::singleShot(500, &loop7, SLOT(quit()));
    loop7.exec();
    // 选出下一次的庄。
    // 从本轮得分最高的人中选出一位。
    std::vector<int> scores;
    int maxscore = score1;
    scores.push_back(1);
    if (score2 == maxscore)
    {
        scores.push_back(2);
    }
    else if (score2 > maxscore)
    {
        scores.clear();
        scores.push_back(2);
    }
    if (score3 == maxscore)
    {
        scores.push_back(3);
    }
    else if (score3 > maxscore)
    {
        scores.clear();
        scores.push_back(3);
    }
    if (score4 == maxscore)
    {
        scores.push_back(4);
    }
    else if (score4 > maxscore)
    {
        scores.clear();
        scores.push_back(4);
    }
    srand((unsigned int)(time(NULL)));
    int roll = rand() % scores.size();
    int nextlord = scores.at(roll);
    if (nextlord == 1)
    {
        lord = Self;
    }
    else if (nextlord == 2)
    {
        lord = Left;
    }
    else if (nextlord == 3)
    {
        lord = Right;
    }
    else
    {
        lord = Opposite;
    }
}

//绘制手牌区手牌，也是基于斗地主的代码(但21点和斗地主的牌的排法并不一样)。
void BlackjackOffline::PaintHandCards(HandCards& handcards)
{
    int size = handcards.GetCards().size();
    Seat seat = handcards.GetSeat();
    switch((int)seat)
    {
        case Seat::Self:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->setFixedSize(80,105);
                handcards.GetCards().at(i)->move(this->width()*0.5 + 80/4*(i-size/2-2),this->height()*0.5+220);
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Left:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->setFixedSize(60,79);
                handcards.GetCards().at(i)->move(this->width()*0.5-handcards.GetCards().at(i)->width()*0.5-500 + 60/4*(i-size/2),this->height()*0.5-50);
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Right:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->setFixedSize(60,79);
                handcards.GetCards().at(i)->move(this->width()*0.5-handcards.GetCards().at(i)->width()*0.5+500 + 60/4*(i-size/2),this->height()*0.5-50);
                handcards.GetCards().at(i)->raise();
            }
        break;
        case Seat::Opposite:
            for(int i=0; i<size; i++)
            {
                handcards.GetCards().at(i)->setFixedSize(60,79);
                handcards.GetCards().at(i)->move(this->width()*0.5 + 60/4*(i-size/2-2),this->height()*0.5-270);
                handcards.GetCards().at(i)->raise();
            }
        break;
    }
}
