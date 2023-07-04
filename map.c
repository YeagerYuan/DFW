#include <stdio.h>
#include <malloc.h>
#include "base.h"


GAME * gameInit() {

    GAME *game = (GAME *)malloc(sizeof(GAME));
    game->map[0].BlockType = STPOINT;

    for (int i = 1; i <= 28; ++i)
    {
        game->map[i].BlockType = LAND;
        game->map[i].HouseOwnerId = -1;
    }

    game->map[14].BlockType = HOSPITAL;
    game->map[28].BlockType = PROPHOUSE;

    for (int i = 29; i <= 63; ++i)
    {
        game->map[i].BlockType = LAND;
        game->map[i].HouseOwnerId = -1;
    }

    game->map[35].BlockType = GIFTHOUSE;
    game->map[49].BlockType = JAIL;
    game->map[63].BlockType = MAGICHOUSE;

    for (int i = 64; i <= 69; ++i)
    {
        game->map[i].BlockType = MINERALFILED;
    }
}
