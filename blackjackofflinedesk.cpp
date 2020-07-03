#include "blackjackofflinedesk.h"
#include "cardpicture.h"
#include "shuffle.h"
#include <vector>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QTimer>
#include <QPushButton>
#include "mypushbutton.h"

BlackjackOfflineDesk::BlackjackOfflineDesk(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200, 800);  // 设置窗口大小
    blackjackoffline = new BlackjackOffline;    // 生成游戏
    // 进行窗口布局。这里参考了斗地主部分的代码。
    MyPushButton* startButton = new MyPushButton(":/Image/start_button.png");
    startButton->setParent(this);
    startButton->move(this->width()*0.9-startButton->width()*0.9, this->height()*0.65);
    MyPushButton* ReturnButton = new MyPushButton(":/Image/return_button2.png");
    ReturnButton->setParent(this);
    ReturnButton->move(this->width()*0.9-ReturnButton->width()*0.9, this->height()*0.8);
    connect(startButton, &MyPushButton::MousePress, [&]()
    {
        blackjackoffline->setGeometry(this->geometry());
        this->close();
        blackjackoffline->InitGame();
        blackjackoffline->show();
        blackjackoffline->NewGame();
        bool playing = true;    // 多局游玩
        while (playing)
        {
            blackjackoffline->MainGame();
            // 游戏结束后弹出结束和继续按钮。
            MyPushButton* endButton = new MyPushButton(":/Image/end_game_button.png");
            endButton->setParent(blackjackoffline);
            endButton->move(blackjackoffline->width()*0.5 + 70 - endButton->width()/2, blackjackoffline->height() - 50);
            endButton->show();
            MyPushButton* nextButton = new MyPushButton(":/Image/next_game_button.png");
            nextButton->setParent(blackjackoffline);
            nextButton->move(blackjackoffline->width()*0.5 - 70 - nextButton->width()/2, blackjackoffline->height() - 50);
            nextButton->show();
            QEventLoop loop2;
                connect(endButton, &MyPushButton::MousePress, [&](){
                    playing = false;
                    loop2.exit();
                    emit ReturnSignal();
                    blackjackoffline->ExitGame();
                    blackjackoffline->close();
                });
                connect(nextButton, &MyPushButton::MousePress, [&](){
                    loop2.exit();
                });
            loop2.exec();
            delete endButton;
            delete nextButton;
            if (playing)
            {
                blackjackoffline->NextGame();
            }
        }
    });
    connect(ReturnButton, &MyPushButton::MousePress, [=](){
        emit ReturnSignal();
    });
}

void BlackjackOfflineDesk::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/bg.jpg");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}
