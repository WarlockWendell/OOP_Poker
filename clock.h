/**************************************
 * 时钟类，用来控制计时
 * ***********************************/
#ifndef CLOCK_H
#define CLOCK_H

#include <QMainWindow>
#include <QWidget>
#include "cardpicture.h"

class Clock : public QWidget
{
    Q_OBJECT
    int time;  //这个时钟剩余的时间
    Seat seat; //这个时钟显示的位置
public:
    Clock(int time,Seat s, QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
signals:

};

#endif // CLOCK_H
