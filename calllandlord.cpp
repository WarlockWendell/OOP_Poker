#include "calllandlord.h"
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QEventLoop>

static int times;
//如果不选择当地主就返回Central
void CallSelf(LandlordOffline& l)  //自己叫地主
{
    times ++;
    qDebug()<<times;
    if(times == 5) //已经绕了一圈，没有人叫地主
    {
        emit l.LandlordSelected();
        QLabel* landlordLabel = new QLabel(&l);
        landlordLabel->setPixmap(QPixmap(":/Image/landlord_image.png"));
        landlordLabel->setFixedSize(50,46);
        landlordLabel->move(550,750);
        landlordLabel->show();
        l.landlord = Seat::Self;
        qDebug()<<"wawa";
        return;
    }
    else if(times < 5)
    {
        Clock* clk = new Clock (15,Self,&l); //时钟

        //叫地主的按钮
        MyPushButton* Call = new MyPushButton(":/Image/button.png");//叫地主按钮
        Call->setParent(&l);
        Call->setFixedSize(140,46);
        QLabel* CallText = new QLabel(Call);
        CallText->setFixedSize(140,46);
        CallText->setAlignment(Qt::AlignCenter);
        CallText->setText("叫地主");
        CallText->setStyleSheet("color:green");
        CallText->setFont(QFont("黑体",20,QFont::Light));
        Call->move((&l)->width()/2 - 220, (&l)->height()/2 + 150);

        //不叫地主的按钮
        MyPushButton* NotCall = new MyPushButton(":/Image/button.png");//不叫按钮
        NotCall->setParent(&l);
        NotCall->setFixedSize(140,43);
        QLabel* NotCallText = new QLabel(NotCall);
        NotCallText->setFixedSize(140,46);
        NotCallText->setAlignment(Qt::AlignCenter);
        NotCallText->setText("不叫");
        NotCallText->setStyleSheet("color:green");
        NotCallText->setFont(QFont("黑体",20,QFont::Light));
        NotCall->move((&l)->width()/2 + 70, (&l)->height()/2 + 150);

        Call->show();
        NotCall->show();

        clk->StartClock();  //开始计时

        QObject::connect(Call,&MyPushButton::MousePress,[&]()
        {
            //如果叫地主的话，就显示地主的图标
            QLabel* landlordLabel = new QLabel(&l);
            landlordLabel->setPixmap(QPixmap(":/Image/landlord_image.png"));
            landlordLabel->setFixedSize(50,46);
            landlordLabel->move(550,750);
            landlordLabel->show();
            l.landlord = Seat::Self;
            emit l.LandlordSelected();
        });

        QObject::connect(NotCall,&MyPushButton::MousePress,[&](){
            QLabel* notCallLabel = new QLabel(&l);
            notCallLabel->setText("不叫");
            notCallLabel->setStyleSheet("color:yellow");
            notCallLabel->setFont(QFont("黑体",28,QFont::Light));
            notCallLabel->move(l.width()/2-notCallLabel->width()/2,l.height()/2+150);
            notCallLabel->show();
            QObject::connect(&l,&LandlordOffline::LandlordSelected,notCallLabel,&QLabel::close);
            CallRight(l);
            qDebug()<<"bujiao";
        });

        QObject::connect(clk,&Clock::TimeOut,[&](){
            QLabel* notCallLabel = new QLabel(&l);
            notCallLabel->setText("不叫");
            notCallLabel->setStyleSheet("color:yellow");
            notCallLabel->setFont(QFont("黑体",28,QFont::Light));
            notCallLabel->move(l.width()/2-notCallLabel->width()/2,l.height()/2+150);
            notCallLabel->show();
            QObject::connect(&l,&LandlordOffline::LandlordSelected,notCallLabel,&QLabel::close);
            CallRight(l);
            qDebug()<<"Self bu";
        });
        QObject::connect(NotCall,&MyPushButton::MousePress,Call,&MyPushButton::close);
        QObject::connect(NotCall,&MyPushButton::MousePress,NotCall,&MyPushButton::close);
        QObject::connect(Call,&MyPushButton::MousePress,Call,&MyPushButton::close);
        QObject::connect(Call,&MyPushButton::MousePress,NotCall,&MyPushButton::close);
        QObject::connect(clk,&Clock::TimeOut,Call,&MyPushButton::close);
        QObject::connect(clk,&Clock::TimeOut,NotCall,&MyPushButton::close);
        QObject::connect(Call,&MyPushButton::MousePress,clk,&Clock::StopClock);
        QObject::connect(NotCall,&MyPushButton::MousePress,clk,&Clock::StopClock);
    }
    else
        return;

}

void CallLeft(LandlordOffline& l) //上家叫地主
{
    times ++;
    qDebug()<<times;
    if(times == 5) //已经绕了一圈，没有人叫地主
    {
        QLabel* landlordLabel = new QLabel(&l);
        landlordLabel->setPixmap(QPixmap(":/Image/landlord_image.png"));
        landlordLabel->setFixedSize(50,46);
        landlordLabel->move(5,375);
        landlordLabel->show();
        l.landlord = Seat::Left;
        emit l.LandlordSelected();
        return;
    }
    else if(times < 5)
    {
        Clock* clk = new Clock (5,Left,&l); //时钟
        clk->StartClock();

        //如果不叫地主
        QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
        {
            QLabel* notCallLabel = new QLabel(&l);
            notCallLabel->setText("不叫");
            notCallLabel->setStyleSheet("color:yellow");
            notCallLabel->setFont(QFont("黑体",28,QFont::Light));
            notCallLabel->move(l.width()/2-notCallLabel->width()/2 - 250,l.height()/2);
            notCallLabel->show();
            QObject::connect(&l,&LandlordOffline::LandlordSelected,notCallLabel,&QLabel::close);
            CallSelf(l);
        });

    }
    else
        return;


}

void CallRight(LandlordOffline& l) //下家叫地主
{
    times ++;
    qDebug()<<times;
    if(times == 5) //已经绕了一圈，没有人叫地主
    {
        QLabel* landlordLabel = new QLabel(&l);
        landlordLabel->setPixmap(QPixmap(":/Image/landlord_image.png"));
        landlordLabel->setFixedSize(50,46);
        landlordLabel->move(1150,375);
        landlordLabel->show();
        l.landlord = Seat::Right;
        emit l.LandlordSelected();
        return;
    }
    else if(times < 5)
    {
        Clock* clk = new Clock (5,Right,&l); //时钟
        clk->StartClock();

        //如果不叫地主
        QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
        {
            QLabel* notCallLabel = new QLabel(&l);
            notCallLabel->setText("不叫");
            notCallLabel->setStyleSheet("color:yellow");
            notCallLabel->setFont(QFont("黑体",28,QFont::Light));
            notCallLabel->move(l.width()/2-notCallLabel->width()/2+250,l.height()/2);
            notCallLabel->show();
            QObject::connect(&l,&LandlordOffline::LandlordSelected,notCallLabel,&QLabel::close);
            CallOpposite(l);
        });

    }
    else
        return;
}

void CallOpposite(LandlordOffline& l) //对门叫地主
{
    times ++;
    qDebug()<<times;
    if(times == 5) //已经绕了一圈，没有人叫地主
    {
        QLabel* landlordLabel = new QLabel(&l);
        landlordLabel->setPixmap(QPixmap(":/Image/landlord_image.png"));
        landlordLabel->setFixedSize(50,46);
        landlordLabel->move(250,100);
        landlordLabel->show();
        l.landlord = Seat::Opposite;
        emit l.LandlordSelected();
        return;
    }
    else if(times<5)
    {

        Clock* clk = new Clock (5,Opposite,&l); //时钟
        clk->StartClock();
        //如果不叫地主
        QObject::connect(clk,&Clock::TimeOut,[&]() //这里要加判定条件
        {
            QLabel* notCallLabel = new QLabel(&l);
            notCallLabel->setText("不叫");
            notCallLabel->setStyleSheet("color:yellow");
            notCallLabel->setFont(QFont("黑体",28,QFont::Light));
            notCallLabel->move(l.width()/2-notCallLabel->width()/2,l.height()/2-150);
            notCallLabel->show();
            QObject::connect(&l,&LandlordOffline::LandlordSelected,notCallLabel,&QLabel::close);
            CallLeft(l);
        });
    }
    else return;
}

static void Switch(LandlordOffline& l,Seat seat)
{
    switch ((int) seat)
    {
        case Seat::Self:
        {
            CallSelf(l);
            break;
        }
        case Seat::Left:
        {
            CallLeft(l);
            break;
        }
        case Seat::Right:
        {
            CallRight(l);
            break;
        }
        case Seat::Opposite:
        {
            CallOpposite(l);
            break;
        }
    }
}

void CallLandlord(LandlordOffline& l)
{
    l.landlord = Seat::Central;
    srand((unsigned int)(time(NULL)));
    Seat begin = (Seat) (rand() % 4); //随机生成一个起始叫地主的
    int begin_bak = (int)begin; //暂存一下起始叫地主的
    times = 0; //叫地主的次数
    Switch(l,(Seat)begin_bak); //第一次叫牌

    //等待叫地主完成
    QEventLoop loop;
    QObject::connect(&l,&LandlordOffline::LandlordSelected,&loop,&QEventLoop::quit);
    loop.exec();

    //加底牌
    l.AddBottomCards(l.landlord);
    switch ((int)l.landlord)
    {
        case Seat::Self:
            l.PaintHandCards(*(l.selfCards));
        break;
        case Seat::Left:
            l.PaintHandCards(*(l.leftCards));
        break;
        case Seat::Right:
            l.PaintHandCards(*(l.rightCards));
        break;
        case Seat::Opposite:
            l.PaintHandCards(*(l.oppositeCards));
        break;
    }
}

