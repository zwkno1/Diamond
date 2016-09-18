#include "diamondgame.h"
#include <iostream>

struct DiamondFactory
{
    static Diamond * createDiamond(int idx)
    {
        static Diamond diamonds[7] =
        {
            {
                2, 2,
                {
                    { 1, 1, 0, 0},
                    { 1, 1, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                1, 4,
                {
                    { 2, 2, 2, 2},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                2, 3,
                {
                    { 0, 3, 3, 0},
                    { 3, 3, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                2, 3,
                {
                    { 4, 4, 0, 0},
                    { 0, 4, 4, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                2, 3,
                {
                    { 5, 5, 5, 0},
                    { 5, 0, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                2, 3,
                {
                    { 6, 6, 6, 0},
                    { 0, 0, 6, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
            {
                2, 3,
                {
                    { 7, 7, 7, 0},
                    { 0, 7, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
            },
        };
        idx = idx%7;
        //std::cout << "create diamond: "<< idx << "," << diamonds[idx].width << ", " << diamonds[idx].height << std::endl;
        return &diamonds[idx];
    }
};

static void DiamondGameCreateDiamond(DiamondGame * game)
{
    game->diamond = DiamondFactory::createDiamond(rand());
    game->diamond_dir = rand() % DirNum;

    game->diamond_x = game->width/2;
    if(game->diamond_dir == Up || game->diamond_dir == Down)
    {
        game->diamond_y = -game->diamond->height;
    }
    else
    {
        game->diamond_y = -game->diamond->width;
    }
}

static void eatDiamond(DiamondGame * game)
{
    int x, y;
    for(int i = 0; i < Diamond::MaxWidth; ++i)
    {
        for(int j = 0; j < Diamond::MaxHeight; ++j)
        {
            x = game->diamond_x + i;
            y = game->diamond_y + j;
            char mask = DiamondGameGetDiamondMask(game->diamond, i, j, game->diamond_dir);
            if(mask)
            {
                if(x < 0 || x >= game->width || y < 0 || y >= game->height)
                {
                    game->state = GameOver;
                }
                else
                {
                    game->blocks[x+y*game->width] = mask;
                }
            }
        }
    }

    int lines_eat = 0;
    static int scores[] = {0, 10, 30, 50, 100};
    if(game->state == GameRunning)
    {
        for(y = game->height -1; y >=0;)
        {
            for(x = 0; x < game->width; ++x)
            {
                if(game->blocks[x+y*game->width] == 0)
                    break;
            }

            if(x == game->width)
            {
                for(int i = y; i > 0; --i)
                {
                    for(int j = 0; j < game->width; ++j)
                        game->blocks[j+i*game->width] = game->blocks[j+(i-1)*game->width];
                }

                for(int i = 0; i < game->width; ++i)
                {
                    game->blocks[i] = 0;
                }
                ++lines_eat;
            }
            else
            {
                --y;
            }
        }
        game->score += scores[lines_eat];
    }
}

static int checkCollision(DiamondGame * game)
{
    int x, y;
    for(int i = 0; i < Diamond::MaxWidth; ++i)
    {
        for(int j = 0; j < Diamond::MaxHeight; ++j)
        {
            x = game->diamond_x + i;
            y = game->diamond_y + j;
            if(DiamondGameGetDiamondMask(game->diamond, i, j, game->diamond_dir))
            {
                if(y >= 0)
                {
                    if(x < 0 || x >= game->width || y >= game->height || game->blocks[x + y * game->width])
                        return 1;
                }
            }
        }
    }
    return 0;
}

DiamondGame * DiamondGameCreate()
{
    DiamondGame *game = new DiamondGame;
    return game;
}

void DiamondGameDestroy(DiamondGame *game)
{
    delete game;
}

void DiamondGameInit(DiamondGame * game, int width, int height)
{
    game->width = width;
    game->height = height;
    game->state = GameRunning;
    game->blocks = std::vector<char>(width*height, 0);
    game->diamond = 0;
    game->score = 0;
    DiamondGameCreateDiamond(game);
}

int DiamondGameStep(DiamondGame * game)
{
    if(game->state != GameRunning)
        return game->state;

    ++game->diamond_y;
    if(checkCollision(game))
    {
        --game->diamond_y;
        eatDiamond(game);
        if(game->state == GameRunning)
            DiamondGameCreateDiamond(game);
    }

    return game->state;
}

int DiamondGameControl(DiamondGame * game, ControlType type)
{
    if(game->state != GameRunning)
    {
        if(game->state == GamePaused && type == PauseResume)
        {
            game->state = GameRunning;
            return 1;
        }
        return 0;
    }

    int x = game->diamond_x;
    int y = game->diamond_y;
    int dir = game->diamond_dir;
    switch(type)
    {
    case ChangeDirection:
        game->diamond_dir = (dir+1)%DirNum;
        break;
    case MoveLeft:
        --game->diamond_x;
        break;
    case MoveRight:
        ++game->diamond_x;
        break;
    case MoveDown:
        while(!checkCollision(game))
        {
            ++game->diamond_y;
        }
        --game->diamond_y;
        return 1;
        break;
    case PauseResume:
        game->state = GamePaused;
        return 1;
        break;
    }

    if(checkCollision(game))
    {
        game->diamond_x = x;
        game->diamond_y = y;
        game->diamond_dir = dir;
        return 0;
    }
    std::cout << "dir: " << game->diamond_dir << std::endl;
    return 1;
}

char DiamondGameGetDiamondMask(Diamond * diamond, int x, int y, int dir)
{
    int x1,y1;

    switch(dir)
    {
    case Up:
    case Down:
        if(x < 0 || x >= diamond->width || y < 0 || y >= diamond->height)
            return 0;
        break;
    case Left:
    case Right:
        if(x < 0 || x >= diamond->height || y < 0 || y >= diamond->width)
            return 0;
        break;
    default:
        return 0;
    }

    switch(dir)
    {
    case Up:
        x1 = x;
        y1 = y;
        break;
    case Down:
        x1 = diamond->width-x-1;
        y1 = diamond->height-y-1;
        break;
    case Left:
        x1 = diamond->width-y-1;
        y1 = x;
        break;
    case Right:
        x1 = y;
        y1 = diamond->height-x-1;
        break;
    default:
        return 0;
    }
    std::cout << "(" << x << "," << y << ") -> (" << x1 << "," << y1 << ")" << std::endl;
    return diamond->blocks[x1][y1];
}
