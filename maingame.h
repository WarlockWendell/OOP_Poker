/*****************************
 * 斗地主
 * **************************/

#ifndef MAINGAME_H
#define MAINGAME_H

#include <QMainWindow>

class MainGame : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainGame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void StartGame();
signals:

public slots:
    void InitGame();
};

#endif // MAINGAME_H
