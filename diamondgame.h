#ifndef DIAMONDGAME_H
#define DIAMONDGAME_H

#include <vector>

enum Direction
{
    Up,
    Left,
    Down,
    Right,
    DirNum
};

enum ControlType
{
    ChangeDirection,
    MoveLeft,
    MoveRight,
    MoveDown,
    PauseResume,
};

enum GameState
{
    GameRunning,
    GameOver,
    GamePaused,
};

struct Diamond
{
    enum { MaxWidth = 4, MaxHeight = 4 };
    int width;
    int height;
    char blocks[MaxWidth][MaxHeight];
};

struct DiamondGame
{
    int width;
    int height;
    GameState state;
    std::vector<char> blocks;
    Diamond * diamond;
    int diamond_dir;
    int diamond_x;
    int diamond_y;
    int score;
};

DiamondGame *DiamondGameCreate();

void DiamondGameDestroy(DiamondGame * game);

void DiamondGameInit(DiamondGame * game, int width, int height);

int DiamondGameStep(DiamondGame * game);

int DiamondGameControl(DiamondGame * game, ControlType type);

char DiamondGameGetDiamondMask(Diamond * diamond, int x, int y, int dir);

#endif // DIAMONDGAME_H
