#ifndef DESKSCENE_H
#define DESKSCENE_H

#include <QObject>
#include <QWidget>
#include "cardpicture.h"
#include "maingame.h"
class DeskScene : public QWidget
{
    Q_OBJECT
    MainGame* mainGame = nullptr;
public:
    explicit DeskScene(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event);
signals:
    void ReturnSignal();
};

#endif // DESKSCENE_H
