#include "mypushbutton.h"
#include <QDebug>
MyPushButton::MyPushButton(QString normalPic, QString pressPic):NormalPic(normalPic),PressPic(pressPic)
{
    QPixmap pix;
    pix.load(NormalPic);
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

void MyPushButton::AlterPicture(QString pic)
{
    QPixmap pix;
    pix.load(pic);
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    if(PressPic != "") //没有第二张图片不需要换
    {

        QPixmap pix;
        pix.load(PressPic);
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    emit MousePress();
    return QPushButton::mousePressEvent(event);
}


void MyPushButton::mouseReleaseEvent(QMouseEvent* event)
{

    if(PressPic != "") //没有第二张图片不需要换
    {

        QPixmap pix;
        pix.load(NormalPic);
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    emit MouseRelease();
    return QPushButton::mousePressEvent(event);
}
