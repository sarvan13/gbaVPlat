#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>
#include "toolbox.h"

enum spriteSize{
    SPRIT_SIZE_8X8,
    SPRIT_SIZE_16X16,
    SPRIT_SIZE_32X32,
    SPRIT_SIZE_64X64,
    SPRIT_SIZE_16X8,
    SPRIT_SIZE_32X8,
    SPRIT_SIZE_32X16,
    SPRIT_SIZE_64X32,
    SPRIT_SIZE_8X16,
    SPRIT_SIZE_8X32,
    SPRIT_SIZE_16X32,
    SPRIT_SIZE_32X64
};

INLINE enum spriteSize getSpriteSize(OBJ_ATTR* object);

INLINE enum spriteSize getSpriteSize(OBJ_ATTR* object)
{
    return (object->attr0 >> 14 | ((object->attr1 >> 14) << 2));
}

OBJ_ATTR* detectCollision(OBJ_ATTR* player, OBJ_ATTR* objectList, int objectCount);

#endif