/*****************************************
 * 单机斗地主桌面
 * **************************************/
#ifndef LANDLORDOFFLINEDESK_H
#define LANDLORDOFFLINEDESK_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include "cardpicture.h"
#include "landlordoffline.h"
class LandlordOfflineDesk : public QWidget
{
    Q_OBJECT
    LandlordOffline* landlordOffline = nullptr;
public:
    explicit LandlordOfflineDesk(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
signals:
    void ReturnSignal();
};

#endif // LandlordOfflineDesk_H
