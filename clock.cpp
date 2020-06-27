#include "clock.h"
#include <QPainter>
Clock::Clock(int t, Seat s, QWidget *parent) : QWidget(parent),time(t),seat(s)
{
}

void Clock::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);  //定义一个画家
    QPixmap clockPic(":/Image/clockPic.png"); //正的时钟
    //QPixmap clockPic_Left(":/Image/clockPic_left.png"); //向左边歪的时钟
    //QPixmap clockPic_Right(":/Image/clockPic_right.png"); //向右边歪的时钟

    painter.drawPixmap(this->rect(),clockPic);
}
