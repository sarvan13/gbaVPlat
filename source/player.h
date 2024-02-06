#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>
#include "toolbox.h"

typedef struct OBJ_POS
{
    /* data */
    int x;
    int y;
    int width;
    int height;
} OBJ_POS;

enum spriteSize{
    SPRITE_SIZE_8X8,
    SPRITE_SIZE_16X16,
    SPRITE_SIZE_32X32,
    SPRITE_SIZE_64X64,
    SPRITE_SIZE_16X8,
    SPRITE_SIZE_32X8,
    SPRITE_SIZE_32X16,
    SPRITE_SIZE_64X32,
    SPRITE_SIZE_8X16,
    SPRITE_SIZE_8X32,
    SPRITE_SIZE_16X32,
    SPRITE_SIZE_32X64
};

INLINE enum spriteSize getSpriteSize(OBJ_ATTR* object);

INLINE enum spriteSize getSpriteSize(OBJ_ATTR* object)
{
    return (object->attr0 >> 14 | ((object->attr1 >> 14) << 2));
}

OBJ_ATTR* detectCollision(OBJ_ATTR* player, OBJ_ATTR* objectList, int objectCount);

OBJ_POS* getObjectPosition(OBJ_ATTR* player);

#endif