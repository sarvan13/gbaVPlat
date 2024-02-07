#include "player.h"
#include <stdlib.h>

void testSpriteSizeEnum(OBJ_ATTR* player)
{
    if (getSpriteSize(player) == SPRITE_SIZE_16X16)
    {
        obj_set_pos(player, 20, 20);
    }
}

// Creates an OBJ_POS struct on the heap - Memory management must be handled by the user
OBJ_POS* getObjectPosition(OBJ_ATTR* player)
{
    int playerX, playerY, playerWidth, playerHeight;

    playerY = player->attr0 & ATTR0_Y_MASK;
    playerX = player->attr1 & ATTR1_X_MASK;

    //Set Width
    if (getSpriteSize(player) == SPRITE_SIZE_8X8 || getSpriteSize(player) == SPRITE_SIZE_8X16
         || getSpriteSize(player) == SPRITE_SIZE_8X32)
    {
        playerWidth = 8;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_16X8 || getSpriteSize(player) == SPRITE_SIZE_16X16
         || getSpriteSize(player) == SPRITE_SIZE_16X32)
    {
        playerWidth = 16;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_32X8 || getSpriteSize(player) == SPRITE_SIZE_32X16
         || getSpriteSize(player) == SPRITE_SIZE_32X32 || getSpriteSize(player) == SPRITE_SIZE_32X64)
    {
        playerWidth = 32;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_64X64 || getSpriteSize(player) == SPRITE_SIZE_64X32)
    {
        playerWidth = 64;
    }

    //Set Height
    if (getSpriteSize(player) == SPRITE_SIZE_8X8 || getSpriteSize(player) == SPRITE_SIZE_16X8
         || getSpriteSize(player) == SPRITE_SIZE_32X8)
    {
        playerHeight = 8;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_8X16 || getSpriteSize(player) == SPRITE_SIZE_16X16
         || getSpriteSize(player) == SPRITE_SIZE_32X16)
    {
        playerHeight = 16;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_8X32 || getSpriteSize(player) == SPRITE_SIZE_16X32
         || getSpriteSize(player) == SPRITE_SIZE_32X32 || getSpriteSize(player) == SPRITE_SIZE_64X32)
    {
        playerHeight = 32;
    }
    else if (getSpriteSize(player) == SPRITE_SIZE_64X64 || getSpriteSize(player) == SPRITE_SIZE_32X64)
    {
        playerHeight = 64;
    }

    OBJ_POS *playerPosition = malloc(sizeof(OBJ_POS));
    playerPosition->x = playerX;
    playerPosition->y = playerY;
    playerPosition->width = playerWidth;
    playerPosition->height = playerHeight;

    return playerPosition;
}

OBJ_ATTR* detectCollision(OBJ_ATTR* player, OBJ_ATTR* objectList, int objectCount)
{
    OBJ_POS *playerPosition = getObjectPosition(player);

    for (int i = 0; i < objectCount; i++)
    {
        if (&objectList[i] != player)
        {
            OBJ_POS *objectPosition = getObjectPosition(&objectList[i]);

            // Check if players bounding box overlaps with objects
            if (playerPosition->x < objectPosition->x + objectPosition->width &&
                playerPosition->x + playerPosition->width > objectPosition->x &&
                playerPosition->y < objectPosition->y + objectPosition->height &&
                playerPosition->y + playerPosition->height > objectPosition->y)
            {
                free(objectPosition);
                return &objectList[i];
            }
            free(objectPosition);
        }
    }

    free(playerPosition);
    return NULL;
}

bool isGrounded(OBJ_ATTR* player, OBJ_ATTR* objectList, int objectCount)
{
    OBJ_POS *playerPosition = getObjectPosition(player);
    int playerBottom = playerPosition->y + playerPosition -> height;

    if (playerBottom == 160) 
    {
        free(playerPosition);
        return 1;
    }
    else
    {
        OBJ_ATTR boxCast = {player->attr0, player->attr1, player->attr2, 0};
        boxCast.attr0 = BFN_SET(boxCast.attr0, playerPosition->y + 7, ATTR0_Y);
        OBJ_ATTR *intersectedObj = detectCollision(&boxCast, objectList + 1, objectCount - 1);

        if (intersectedObj)
        { 
            OBJ_POS *intersectedObjPos = getObjectPosition(intersectedObj);
            if (playerBottom == intersectedObjPos->y)
            {
                free(playerPosition);
                free(intersectedObjPos);
                return 1;
            }
            
            free(intersectedObjPos);
        }
    }
    free(playerPosition);

    return 0;
}