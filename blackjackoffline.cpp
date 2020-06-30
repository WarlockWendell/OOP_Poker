#include "blackjackoffline.h"
#include "handcards.h"
#include "card.h"
#include "cardpicture.h"
#include "shuffle.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QPainter>
#include <QEventLoop>
#include <QThread>
#include <ctime>

BlackjackOffline::BlackjackOffline(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(1200,800);
    //四个玩家的手牌区
    selfCards = new HandCardsBlackjack(Seat::Self);
    leftCards = new HandCardsBlackjack(Seat::Left);
    rightCards = new HandCardsBlackjack(Seat::Right);
    oppositeCards = new HandCardsBlackjack(Seat::Opposite);
}
