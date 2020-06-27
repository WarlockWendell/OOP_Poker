#include "landlordofflinedesk.h"
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
LandlordOfflineDesk::LandlordOfflineDesk(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200,800);  //窗口大小
    landlordOffline = new LandlordOffline;
    MyPushButton* startButton = new MyPushButton(":/Image/start_button.png");
    startButton->setParent(this);
    startButton->move(this->width()*0.5-startButton->width()*0.5, this->height()*0.3);
    MyPushButton* ReturnButton = new MyPushButton(":/Image/return_button2.png");
    ReturnButton->setParent(this);
    ReturnButton->move(this->width()*0.5-ReturnButton->width()*0.5, this->height()*0.6);
    connect(startButton,&MyPushButton::MousePress,[&](){
        landlordOffline->setGeometry(this->geometry());
        this->close();
        landlordOffline->InitGame();
        landlordOffline->show();
        landlordOffline->StartGame();
        landlordOffline->MainGame();
    });
    connect(ReturnButton,&MyPushButton::MousePress,[=](){
        emit ReturnSignal();
    });
}

void LandlordOfflineDesk::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPixmap pix;
    pix.load(":/Image/desk_background.png");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),pix);
}

