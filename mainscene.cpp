#include "mainscene.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include "mypushbutton.h"
MainScene::MainScene(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1200,800);  //窗口大小
    //主界面的三个按钮
    MyPushButton* OnlineButton = new MyPushButton(":/Image/online_button.png");
    MyPushButton* OfflineButton = new MyPushButton(":/Image/offline_button.png");
    MyPushButton* QuitButton = new MyPushButton(":/Image/quit_button1.png");
    OnlineButton->setParent(this);
    OfflineButton->setParent(this);
    QuitButton->setParent(this);
    OnlineButton->move(this->width()*0.5-OnlineButton->width()*0.5, this->height()*0.3);
    OfflineButton->move(this->width()*0.5-OfflineButton->width()*0.5, this->height()*0.5);
    QuitButton->move(this->width()*0.5-QuitButton->width()*0.5, this->height()*0.7);

 //监听

    // 离开游戏
    connect(QuitButton,&MyPushButton::MousePress,[=](){
        this->close();
    });




    //单机模式
    connect(OfflineButton,&MyPushButton::MousePress,[=](){
       ChooseGameWidget = new ChooseGame(0);
       QTimer::singleShot(200,this,[=](){
           this->close();
           ChooseGameWidget->setGeometry(this->geometry());
           ChooseGameWidget->show();
       });
       //监听来自chooseGame的返回信号
       connect(ChooseGameWidget,&ChooseGame::ReturnSignal,[=](){
               ChooseGameWidget->close();
               this->setGeometry(ChooseGameWidget->geometry());
               this->show();
       });
    });

    //联机模式
    connect(OnlineButton,&MyPushButton::MousePress,[=](){
       //todo: 选择游戏
        ChooseGameWidget = new ChooseGame(1);
        QTimer::singleShot(200,this,[=](){
            this->close();
            ChooseGameWidget->setGeometry(this->geometry());
            ChooseGameWidget->show();
        });
        //监听来自chooseGame的返回信号
        connect(ChooseGameWidget,&ChooseGame::ReturnSignal,[=](){
                ChooseGameWidget->close();
                this->setGeometry(ChooseGameWidget->geometry());
                this->show();
        });
    });


}


void MainScene::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this); //定义画家
    QPixmap background;
    background.load(":/Image/main_background.png");
    painter.drawPixmap(this->rect(),background);
}
