#include "gameover.h"
#include "landlordofflinedesk.h"
#include <QTimer>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>

//游戏结束界面
GameOver::GameOver(bool isWin, QWidget *parent) : QWidget(parent),isWinFlag(isWin)
{ 
    //设置窗口属性
    setFixedSize(1200,800);
    setWindowIcon(QIcon(":/Image/icon_ddz.PNG")); //设置图标
    setWindowTitle("斗地主");

    //根据胜负设置背景图片
    isWinLabel = new QLabel(this);
    if(isWin){
        isWinLabel->setPixmap(QPixmap(":/Image/victory.PNG"));
    }
    else{
        isWinLabel->setPixmap(QPixmap(":/Image/lose.PNG"));
    }
    isWinLabel->setScaledContents(true);
    isWinLabel->resize(this->size());

    //按钮初始化
    MyPushButton *GameAgainButton = new MyPushButton(":/Image/again.PNG");
    MyPushButton *GameExitButton = new MyPushButton(":/Image/exit.PNG");
    MyPushButton *ReturnButton = new MyPushButton(":/Image/return.PNG");
    GameAgainButton->setParent(this);
    GameExitButton->setParent(this);
    ReturnButton->setParent(this);
    GameAgainButton->resize(200,100);
    GameAgainButton->move(this->width()*0.5-GameAgainButton->width()*0.5,this->height()*0.6);
    GameExitButton->move(this->width()*0.5-GameExitButton->width()*0.5,this->height()*0.75);
    ReturnButton->move(this->width()*0.9,20);
    //监听信号
    //返回，点击返回按钮就回到mainScene
    connect(ReturnButton, &MyPushButton::MousePress, [=](){
            emit ReturnSignal();
    });
    //再来一局
    connect(GameAgainButton, &MyPushButton::MousePress, this, [=](){
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
    //退出游戏
    connect(GameExitButton, &MyPushButton::MousePress, this, [=](){
        this->close();});
}


//    测试代码：暂时利用mainscene的退出按钮进行测试游戏结束功能// 离开游戏
//    connect(QuitButton,&MyPushButton::MousePress,[=](){
//       // this->close();
//        GameOverWidget = new GameOver(0);
//        QTimer::singleShot(200,this,[=](){
//            this->hide();
//            GameOverWidget->show();
//        });
//        //监听来自GameOver的返回信号
//        connect(GameOverWidget,&GameOver::ReturnSignal,[=](){
//                GameOverWidget->close();
//                this->setGeometry(GameOverWidget->geometry());
//                this->show();
//        });
//    });
