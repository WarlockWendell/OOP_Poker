 #include "choosegame.h"
#include <QTimer>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include "cardpicture.h"
ChooseGame::ChooseGame(int gameType,QWidget* parent):QWidget(parent),GameType(gameType)
{
    this->setFixedSize(1200,800);
    MyPushButton* LandlordButton = new MyPushButton(":/Image/landlord_button.png");
    MyPushButton* BlackJackButton = new MyPushButton(":/Image/blackjack_button.png");
    MyPushButton* ReturnButton = new MyPushButton(":/Image/return_button.png");
    LandlordButton->setParent(this);
    BlackJackButton->setParent(this);
    ReturnButton->setParent(this);
    LandlordButton->move(this->width()*0.5-LandlordButton->width()*0.5, this->height()*0.3);
    BlackJackButton->move(this->width()*0.5-BlackJackButton->width()*0.5, this->height()*0.5);
    ReturnButton->move(this->width()*0.5-ReturnButton->width()*0.5, this->height()*0.7);

    //监听信号
    //返回，点击返回按钮就回到mainScene
    connect(ReturnButton,&MyPushButton::MousePress,[=](){
            emit ReturnSignal();
    });
    qDebug()<<GameType;

    if(GameType == 0)  //单机模式
    {
        //todo： 单机模式下的两种游戏
        connect(LandlordButton,&MyPushButton::MousePress,[=](){
            qDebug()<<"press offline landlord";
            LandlordOfflineDesk* test_desk = new LandlordOfflineDesk();
            QTimer::singleShot(200,this,[=](){
                this->close();
                test_desk->setGeometry(this->geometry());
                test_desk->show();
            });
            connect(test_desk,&LandlordOfflineDesk::ReturnSignal,[=](){
                test_desk->close();
                this->setGeometry(test_desk->geometry());
                this->show();
            }) ;
        });
        connect(BlackJackButton,&MyPushButton::MousePress,[=](){
            qDebug()<<"press offline blackjack";
            BlackjackOfflineDesk *test_desk = new BlackjackOfflineDesk();
            QTimer::singleShot(200,this,[=](){
                this->close();
                test_desk->setGeometry(this->geometry());
                test_desk->show();
            });
            connect(test_desk,&BlackjackOfflineDesk::ReturnSignal,[=](){
                test_desk->close();
                this->setGeometry(test_desk->geometry());
                this->show();
            }) ;



        });
     }

    else if(GameType == 1)  //联机模式
    {
        //todo： 联机模式下的两种游戏
        connect(LandlordButton,&MyPushButton::MousePress,[=](){
            qDebug()<<"press online landlord";



        });
        connect(BlackJackButton,&MyPushButton::MousePress,[=](){
            qDebug()<<"press online blackjack";

        });
    }
}
void ChooseGame::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this); //定义画家
    QPixmap background;
    background.load(":/Image/main_background.png");
    painter.drawPixmap(this->rect(),background);
}
