/**************************************
 * 时钟类，用来控制计时
 * ***********************************/
#ifndef CLOCK_H
#define CLOCK_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include "cardpicture.h"

class Clock : public QWidget
{
    Q_OBJECT
    int time;  //这个时钟剩余的时间
    Seat seat; //这个时钟显示的位置
    QTimer* timer;
    QLabel* label, *label_gif;
    QPixmap clockPic;
    QMovie* movie;
public:
    Clock(int time = 0,Seat s = Seat::Central, QWidget *parent = nullptr);
    void StartClock();  //开启时钟
private:
    void paintEvent(QPaintEvent *event);
signals:
    void TimeOut();
    void LeftFive();
public slots:
       void timerout();
};

#endif // CLOCK_H
