#include "player.h"

OBJ_ATTR* detectCollision(OBJ_ATTR* player, OBJ_ATTR* objectList, int objectCount)
{
    int playerX, playerY, playerWidth, playerHeight;

    playerY = player->attr0 & ATTR0_Y_MASK;
    playerX = player->attr1 & ATTR1_X_MASK;

    if (getSpriteSize(player) == SPRIT_SIZE_16X8)
    {
        player->attr0 = (player->attr0 & ~ATTR0_Y_MASK) | 15;
        player->attr1 = (player->attr1 & ~ATTR1_X_MASK) | 15;
    }
    else
    {
        player->attr0 = (player->attr0 & ~ATTR0_Y_MASK) | 100;
        player->attr1 = (player->attr1 & ~ATTR1_X_MASK) | 100;
    }
    // for (int i=0; i < objectCount; i++)
    // {
    //     OBJ_AFFINE* nextObject = &objectList[i];

    //     if (nextObject != player)
    //     {
    //         // Check for collision
            
    //     }
    // }

    return NULL;
}