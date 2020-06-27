//造一个按钮类
//传入两张图片的路径，第二张可以省略，表示按钮的图片
//当第二张不省略时，按下时显示第二张，释放后显示第一张图片


#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
class MyPushButton : public QPushButton
{
    Q_OBJECT
    QString NormalPic, PressPic;



public:
    MyPushButton(QString normalPic, QString pressPic = "");
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void MousePress();  //按下的信号
    void MouseRelease(); //鼠标弹起的信号
};

#endif // MYPUSHBUTTON_H
