#ifndef BLACKJACKOFFLINEDESK_H
#define BLACKJACKOFFLINEDESK_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include "cardpicture.h"
#include "blackjackoffline.h"

class BlackjackOfflineDesk : public QWidget
{
    Q_OBJECT
    BlackjackOffline *blackjackoffline = nullptr;
public:
    explicit BlackjackOfflineDesk(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void ReturnSignal();
};

#endif // BLACKJACKOFFLINEDESK_H
