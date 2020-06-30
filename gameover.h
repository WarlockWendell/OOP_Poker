#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QEvent>

class GameOver : public QWidget
{
    Q_OBJECT
    QLabel *isWinLabel; //胜负标签
    bool isWinFlag;
public:
    //构造函数，传入参数isWin代表是否胜利
    GameOver(bool isWin, QWidget *parent = nullptr);
signals:
    void ReturnSignal(); //返回主界面的信号
};

#endif // GAMEOVER_H
