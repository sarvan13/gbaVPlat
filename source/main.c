/*	Coder:		Decay
	E-mail:		thefountainofdecay@hotmail.com
	Home Page:	http://members.tripod.co.uk/GBCE/index.html (note case)
	File:		Main.c
	Date:		02/02/2002
	Purpose:	simple demo showing how to use double buffering in mode 5
	Notes:		do WaitVBlank() before SwapBuffer() else the graphics will flicker,
				which would negate the hassle of double buffering.
				run in mappy then look at the frame buffers :)
*/

//list defines for the screen register at 0x4000000
#define REG_DISPCNT *(unsigned short*)0x4000000
#define MODE_0 0x0	//set mode 0 - tiled, bkgs 0123, no rot/scale
#define MODE_1 0x1	//set mode 1 - tiled, bkgs 012, rot/scale 2
#define MODE_2 0x2	//set mode 2 - tiled, bkgs 23,  rot/scale 23
#define MODE_3 0x3	//set mode 3 - 16bit buffer (enable bkg 2 to use)
#define MODE_4 0x4	//set mode 4 - 8bit buffer, double bufferable
#define MODE_5 0x5	//set mode 5 - 16bit buffer, double bufferable at 160x128
#define BACKBUFFER	0x10	//controls which buffer is active in mode 4 or 5
#define BKG0_ENABLE 0x100	//enable bkg 0
#define BKG1_ENABLE 0x200	//enable bkg 1	 
#define BKG2_ENABLE 0x400	//enable bkg 2
#define BKG3_ENABLE 0x800	//enable bkg 3
#define OBJ_ENABLE 0x1000 	//enable objects
//define vblank reg, contains line vblank currently at
#define	VBLANK_CNT	*(volatile unsigned short*)0x4000006	//the vertical line being draw, volatile as changed outside of code
//list defines for the Video ram buffer at 0x6000000
#define VRAM	0x6000000	//start of VRAM front buffer
#define BACK_VRAM_M5	0x600A000	//start of VRAM back buffer

//globals
unsigned short *VideoBuffer;	//location we are currently drawing to, flips between front and back buffer

void SwapBuffer(void)	//this swaps the locations of the current display and current writing location
{
	if (REG_DISPCNT&BACKBUFFER)		//backbuffer is being displayed so swap
	{
		REG_DISPCNT &= ~BACKBUFFER;	//make display show the frontbuffer (remove backbuffer using it's bitwise compliement)
		VideoBuffer=(unsigned short*)BACK_VRAM_M5;	//make backbuffer the one we draw to
	}
	else		//frontbuffer is being displayed so swap
	{
		REG_DISPCNT|=BACKBUFFER;	//make display show the backbuffer (add backbuffer)
		VideoBuffer=(unsigned short*)VRAM;	//make frontbuffer the one we draw to
	}
}

void WaitVBlank(void)	//waits for vblank to complete
{
	while (VBLANK_CNT<160){}	//do nothing untill vblank has completed
}

int main(void)
{
	unsigned short counter=0;

	REG_DISPCNT=(MODE_5|BKG2_ENABLE);	//set screen mode 5, enable bkg2 for 16bit buffer
	VideoBuffer=(unsigned short*)BACK_VRAM_M5;	//make backbuffer the one we draw to
	//screen shows VRAM (blank), writing to BACK_VRAM_M5 (blank)

	while (counter++<20480)	//128 lines
		*VideoBuffer++=31;	//draw red pixels to where VideoBuffer points to, here it's the backbuffer (see above)
	//screen shows VRAM (blank), writing to BACK_VRAM_M5 (now red lines)

	counter=0;
	WaitVBlank();	//wait for vblank to complete before swapping buffers
	SwapBuffer();	//swap buffers, here we display backbuffer, set drawing to frontbuffer (VRAM)
	//screen shows BACK_VRAM_M5 (red lines), writing to VRAM (blank)

	while (counter++<20480)	//128 lines
		*VideoBuffer++=(31<<10);	//draw blue pixels to where VideoBuffer points to, here it's the VRAM due to the SwapBuffers()
	//screen shows BACK_VRAM_M5 (red lines), writing to VRAM (now blue lines)

	WaitVBlank();	//wait for vblank to complete before swapping buffers
	SwapBuffer();	//swap buffers, here we display VRAM, set drawing to backbuffer (BACK_VRAM_M5)
	//screen shows VRAM (blue lines), writing to BACK_VRAM_M5 (red lines)
	//code loops
}

