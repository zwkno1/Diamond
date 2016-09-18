#include "gamewidget.h"

#include <QPainter>
#include <QKeyEvent>

#include "diamondgame.h"

const int DIAMOND_WIDTH = 16;
const int DIAMOND_HEIGHT = 16;
const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 20;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent)
{
    game_ = DiamondGameCreate();
    DiamondGameInit(game_, 16, 32);
    this->setFixedSize((game_->width+4)*DIAMOND_WIDTH+BOARD_WIDTH*2, game_->height*DIAMOND_HEIGHT+BOARD_HEIGHT*2);
    this->setWindowTitle("Diamond");
    time_id_ = this->startTimer(300);
}

GameWidget::~GameWidget()
{
    DiamondGameDestroy(game_);
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawRect(BOARD_WIDTH, BOARD_HEIGHT, game_->width * DIAMOND_WIDTH,game_->height*DIAMOND_HEIGHT);

    for(int i = 0; i < game_->width; ++i)
    {
        for(int j = 0; j < game_->height; ++j)
        {
            int mask = game_->blocks[i+game_->width*j];
            if(mask)
            {
                drawBlock(p, BOARD_WIDTH+i*DIAMOND_WIDTH, BOARD_HEIGHT+j*DIAMOND_HEIGHT, DIAMOND_WIDTH, DIAMOND_HEIGHT, mask);
            }
        }
    }

    for(int i = 0; i < Diamond::MaxWidth; ++i)
    {
        for(int j = 0; j < Diamond::MaxHeight; ++j)
        {
            int x = game_->diamond_x + i;
            int y = game_->diamond_y + j;
            if(y < 0)
                continue;
            int mask = DiamondGameGetDiamondMask(game_->diamond, i, j, game_->diamond_dir);
            if(mask)
            {
                drawBlock(p, BOARD_WIDTH+x*DIAMOND_WIDTH, BOARD_HEIGHT+y*DIAMOND_HEIGHT, DIAMOND_WIDTH, DIAMOND_HEIGHT, mask);
            }
        }
    }

    for(int i = 0; i < Diamond::MaxWidth; ++i)
    {
        for(int j = 0; j < Diamond::MaxHeight; ++j)
        {
            int x = i;
            int y = j;
            int mask = DiamondGameGetDiamondMask(game_->diamond, i, j, game_->diamond_dir);
            if(mask)
            {
                drawBlock(p, BOARD_WIDTH*2+(x+game_->width)*DIAMOND_WIDTH, BOARD_HEIGHT*2+y*DIAMOND_HEIGHT, DIAMOND_WIDTH, DIAMOND_HEIGHT, mask);
            }
        }
    }

    p.drawText(BOARD_WIDTH, BOARD_HEIGHT*2+game_->height*DIAMOND_HEIGHT, QString("score:%1").arg(game_->score));

    if(game_->state == GameOver)
    {
        p.drawText(BOARD_WIDTH+(game_->width/2-5)*DIAMOND_WIDTH, BOARD_HEIGHT+game_->height/2*DIAMOND_HEIGHT, "Game Over(Press R retart)");
    }
    else if(game_->state == GamePaused)
    {
        p.drawText(BOARD_WIDTH+(game_->width/2-2)*DIAMOND_WIDTH, BOARD_HEIGHT+game_->height/2*DIAMOND_HEIGHT, "Paused");
    }
}

void GameWidget::timerEvent(QTimerEvent *)
{
    DiamondGameStep(game_);
    repaint();
}

void GameWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Up:
        DiamondGameControl(game_, ControlType::ChangeDirection);
        break;
    case Qt::Key_Down:
        DiamondGameControl(game_, ControlType::MoveDown);
        break;
    case Qt::Key_Left:
        DiamondGameControl(game_, ControlType::MoveLeft);
        break;
    case Qt::Key_Right:
        DiamondGameControl(game_, ControlType::MoveRight);
        break;
    case Qt::Key_Space:
        DiamondGameControl(game_, ControlType::PauseResume);
        break;
    case Qt::Key_R:
        DiamondGameInit(game_, game_->width, game_->height);
        break;
    }
}

void GameWidget::drawBlock(QPainter &painter, int x, int y, int w, int h, int type)
{
    Qt::GlobalColor colors[] = { Qt::red, Qt::magenta, Qt::yellow, Qt::green, Qt::cyan, Qt::blue, Qt::black };
    painter.setBrush(colors[type%7]);
    painter.drawRect(x, y, w, h);

}
