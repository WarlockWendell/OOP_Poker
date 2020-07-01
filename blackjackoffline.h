
#ifndef BLACKJACKOFFLINE_H
#define BLACKJACKOFFLINE_H

#include <QMainWindow>
#include <QString>
#include "clock.h"
#include "handcards.h"
#include "cardpicture.h"
#include "handcards_blackjack.h"

class BlackjackOffline : public QMainWindow
{
    Q_OBJECT
    HandCardsBlackjack *selfCards, *leftCards, *rightCards, *oppositeCards; // 手牌
    Seat lord;                  // 庄家
    int player = -1;            // 当前玩家
    std::vector<CardPicture*> cardpictures; // 游戏的牌堆
    std::vector<CardPicture*> usedDeck;     // 使用过的牌的牌堆
    int selfScore = 0, leftScore = 0, rightScore = 0, oppositeScore = 0;    // 总分
    QString selfLabel, leftLabel, rightLabel, oppositeLabel;                // 文本
    void StartGame();           // 游戏初始化
public:
    BlackjackOffline(QWidget *parent = nullptr);    //构造函数
    void NewGame();             // 开始新的游戏，由玩家作为庄家。
    void NextGame();            // 开始新的游戏，由上一局获胜者作为庄家。
    void MainGame();            // 游戏内容
    void ExitGame();            // 退出游戏时进行内存清空
    void PaintHandCards(HandCards& handcards);      //绘制手牌
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void InitGame();  //初始化函数
};

#endif // BLACKJACKOFFLINE_H

