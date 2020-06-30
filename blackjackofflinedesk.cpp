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
    startButton->move(this->width()*0.5-startButton->width()*0.5, this->height()*0.3);
    MyPushButton* ReturnButton = new MyPushButton(":/Image/return_button2.png");
    ReturnButton->setParent(this);
    ReturnButton->move(this->width()*0.5-ReturnButton->width()*0.5, this->height()*0.6);
    connect(startButton, &MyPushButton::MousePress, [&]()
    {
        blackjackoffline->setGeometry(this->geometry());
        this->close();
        blackjackoffline->InitGame();
        blackjackoffline->show();
        blackjackoffline->StartGame();
        blackjackoffline->MainGame();
    });
    connect(ReturnButton, &MyPushButton::MousePress, [=](){
        emit ReturnSignal();
    });
}

void BlackjackOfflineDesk::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/desk_background.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}
