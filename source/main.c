//
// obj_demo.c
// testing various sprite related things
//
// (20031003 - 20060924, Cearn)

#include <string.h>
#include "toolbox.h"
#include "Cube.h"
#include "player.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

// testing a few sprite things
// D-pad: move 
// SELECT: switch palette
// START: toggle mapping mode
// A: horizontal flip
// B: vertical flip
// L & R shift starting tile
void loop()
{
	int x= 96, y= 144;
	int ymax = y;
	bool jump = 0;
	u8 jumpStrength = 2;
	u8 fallStrength = 1;
	u32 tid= 0, pb= 0;		// tile id, pal-bank

	OBJ_ATTR *cube= &obj_buffer[0];
	obj_set_attr(cube, 
		ATTR0_SQUARE,				// Square, regular sprite
		ATTR1_SIZE_16,					// 16x16p, 
		ATTR2_PALBANK(pb) | tid);		// palbank 0, tile 0

	// position sprite (redundant here; the _real_ position
	// is set further down
	obj_set_pos(cube, x, y);

	OBJ_ATTR *platform= &obj_buffer[1];
	obj_set_attr(platform, 
		ATTR0_WIDE,				
		ATTR1_SIZE_32x8,				
		ATTR2_PALBANK(pb) | tid);		

	platform->attr2 = ATTR2_BUILD(4, pb, 0);
	obj_set_pos(platform, 96, 32);

	detectCollision(platform, &obj_buffer[0], 2);

	while(1)
	{
		vid_vsync();
		key_poll();

		// move left/right
		x += 2*key_tri_horz();

		// move up/down
		jump = KEY_DOWN_NOW(KEY_UP) ?  1: 0;
		y = jump ? y - jumpStrength: y + fallStrength;
		y = (y > ymax) ? ymax : y;


		// increment/decrement starting tile with R/L
		tid += bit_tribool(key_hit(-1), KI_R,  KI_L);

		// flip
		if(key_hit(KEY_A))	// horizontally
			cube->attr1 ^= ATTR1_HFLIP;
		if(key_hit(KEY_B))	// vertically
			cube->attr1 ^= ATTR1_VFLIP;
		
		// make it glow (via palette swapping)
		pb= key_is_down(KEY_SELECT) ? 1 : 0;

		// toggle mapping mode
		if(key_hit(KEY_START))
			REG_DISPCNT ^= DCNT_OBJ_1D;

		// Hey look, it's one of them build macros!
		cube->attr2= ATTR2_BUILD(tid, pb, 0);
		obj_set_pos(cube, x, y);

		oam_copy(oam_mem, obj_buffer, 2);	// only need to update one
	}
}

int main()
{
	// Places the glyphs of a 4bpp boxed metroid sprite 
	//   into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], CubeTiles, CubeTilesLen);
	memcpy(&tile_mem[4][4], PlatTiles, CubeTilesLen);
	memcpy(pal_obj_mem, CubePal, CubePalLen);

	oam_init(obj_buffer, 128);
	REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;

	loop();

	while(1);

	return 0;
}
