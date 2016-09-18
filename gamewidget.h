#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class DiamondGame;

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();
protected:
    void paintEvent(QPaintEvent * e);
    void timerEvent(QTimerEvent * e);
    void keyPressEvent(QKeyEvent * e);
    void drawBlock(QPainter & painter, int x, int y, int w, int h, int type);

private:
    DiamondGame *game_;
    int time_id_;
};

#endif // MAINWINDOW_H
