#include "clock.h"
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QEventLoop>
#include <QMovie>
Clock::Clock(int t, Seat s, QWidget *parent) : QWidget(parent),time(t),seat(s)
{
     setFixedSize(50,50);
     //移动到合适的位置
     switch((int)seat)
     {
         case Seat::Self:
            move(parent->width()*0.5-this->width()*0.5,parent->height()*0.5+120);
         break;
         case Seat::Left:
            move(parent->width()*0.5 - 350, parent->height()*0.5-this->height()*0.5);
         break;
         case Seat::Right:
            move(parent->width()*0.5 + 350, parent->height()*0.5-this->height()*0.5);
         break;
         case Seat::Opposite:
            move(parent->width()*0.5-this->width()*0.5,parent->height()*0.5 - 200);
         break;
         case Seat::Central:
            move(parent->width()*0.5 - this->width()*0.5, parent->height()*0.5 - this->height()*0.5);
         break;
     }
     clockPic.load(":/Image/clockPic.png");
     movie = new QMovie(":/Image/clockPic.gif");
     movie->setScaledSize(this->size());
     label_gif = new QLabel(this);
     label_gif->setFixedSize(this->size());
     label_gif->setMovie(movie);
     //标签
     label = new QLabel();
     label->setParent(this);
     label->setText("");
     label->setFixedSize(this->size());
     label->setFont(QFont("Microsoft YaHei",10,25));
     label->setAlignment(Qt::AlignCenter);
     //计时器
     timer = new QTimer(this);
     connect(timer,&QTimer::timeout,this, &Clock::timerout);
     timer->setInterval(1000);
}

void Clock::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);  //定义一个画家
    if(time >= 5)
    {
        painter.drawPixmap(this->rect(),clockPic);
    }
    else
    {
        label_gif->show();
        movie->start();
    }
    label->show();
    label->raise();
}

void Clock::StartClock()
{
    timer->stop();
    timerout();
    timer->start();
    show();
}

void Clock::timerout()
{
    if(time == -1)
    {
        timer->stop();
        close();
        emit TimeOut();
        return;
    }
    if(time == 5)
    {
        emit LeftFive();
    }
    label->setText(QString::number(time));
    time--;
}
