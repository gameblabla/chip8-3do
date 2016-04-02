/*
	Based on a another CHIP8 emulator that came with no license.
	All the modifications to this CHIP8 emulator are copyright (C) 2015 Gameblabla <gameblabla@openmailbox.org>
	and are subject to the MIT license.
	See http://www.opensource.org/licenses/mit-license.php in a web browser for more information.
*/

#ifndef EMBEDDED
	#include <stdio.h>
#endif

#if !defined(THREEDO) || !defined(PCFX)
	#include <time.h>
#endif
#include "chip8.h"

#ifdef EMBEDDED
	#include "rom.h"
#endif

#ifdef THREEDO
	#include "3DO/GestionAffichage.h"
	#include "3DO/GestionSprites.h"
	#include "3DO/GestionTextes.h"
	#include "3DO/system.h"
	#include <filestreamfunctions.h>
#elif defined(PCFX)
	#include "PCFX/functions.h"
	#include "PCFX/input.h"
#else
	#include "SDL/SDL.h"
	#include "SDL/SDL_keysym.h"
	SDL_Surface* screen;
	SDL_Event event;
#endif

struct b_pixel
{
	unsigned short x;
	unsigned short y;
	unsigned char w;
	unsigned char h;
} bounds;

char* keyname;
char* key;
char chr;

#ifdef EMBEDDED
void load_rom(int start) 
{
    int c, i;
	for (i=0; i<ROM_SIZE; i++) 
    {
		c = ROM[i];
        memory[i + start] = c;
    }
}
#endif


void beep() 
{
    if (sound)
    {
#ifdef PCFX
		play_beep();
#else
        printf("BEEP!\n");
#endif
    }
#ifdef PCFX
    else
    {
		stop_beep();
	}
#endif
}

int main(int argc, char* argv[]) 
{
    /* Load ROM */

#ifdef EMBEDDED
    load_rom(0x200);
#elif THREEDO
	Stream* fp;
    fp = OpenDiskStream( "rom.ch8", 4096 );
    ReadDiskStream(fp, (char *) memory + 0x200, 4096 );
    CloseDiskStream(fp);
#else
    FILE* fp;
	fp = fopen("rom.ch8", "rb");
    fread (memory + 0x200,sizeof(memory),1,fp);
	fclose(fp);
#endif

    /* Load hex font */
    for (temp=0; temp < 80; temp++) 
    {
		memory[temp] = chip8_font[temp];
	}
 
	screen_init();
    
    /* Setup some variables */
    running = 1;
    cinst = INST_PER_CYCLE;
    
    while(running)
    {
		Controls();
		
		if (paused && next_opcode) cinst = 1;
        for (ii = 0; ii < cinst; ii++) {
        if (paused) cinst = 0;
        /* It looks better without identation */
        if (pc >= 4096) 
        {
            break;
        }
        opcode = dopcode;
        inst   = opcode & 0xF000;
        pc += 2;
        switch(inst) 
        {
            case 0x0000:
                switch (KK) 
                {
                    case 0x00E0:
                        for (y=0; y<32; y++) 
                        {
                            for (x=0; x<64; x++) 
                            {
                                gfx[y][x] = 0;
                            }
                        }
						update_screen();
                        break;
                    case 0x00EE:
                        pc = stack[sp--];
                        break;
                    default:
                        /* 0NNN - ignored */
                        break;
                }
                break;
            case 0x1000:
                pc = NNN;
                break;
            case 0x2000:
                stack[++sp] = pc;
                pc = NNN;
                break;
            case 0x3000:
                if (V[X] == KK)
                    pc += 2;
                break;
            case 0x4000:
                if (V[X] != KK)
                    pc += 2;
                break;
            case 0x5000:
                if (V[X] == V[Y])
                    pc += 2;
                break;
            case 0x6000:
                V[X] = KK;
                break;
            case 0x7000:
                V[X] += KK;
                break;
            case 0x8000:
                switch (opcode & 0x000F) {
                    case 0x0000:
                        V[X] = V[Y];
                        break;
                    case 0x0001:
                        V[X] |= V[Y];
                        break;
                    case 0x0002:
                        V[X] &= V[Y];
                        break;
                    case 0x0003:
                        V[X] ^= V[Y];
                        break;
                    case 0x0004:
                        temp = V[X] + V[Y];
                        V[0xF] = (temp > 0xFF);
                        V[X] = temp;
                        break;
                    case 0x0005:
                        temp = V[X] - V[Y];
                        V[0xF] = (V[X] > V[Y]);
                        V[X] = temp;
                        break;
                    case 0x0006:
                        V[0xF] = V[X] & 1;
                        V[X] >>= 1;
                        break;
                    case 0x0007:
                        temp = V[Y] - V[X];
                        V[0xF] = (V[Y] > V[X]);
                        V[X] = temp;
                        break;
                    case 0x000E:
                        V[0xF] = (V[X] & 128) >> 7;
                        V[X] <<= 1;
                        break;
                    default:
                        printf("Unknown opcode: 0x%X\n", opcode);
                        break;
                }
                break;
            case 0x9000:
                if (V[X] != V[Y])
                    pc += 2;
                break;
            case 0xA000:
                I = NNN;
                break;
            case 0xBa000:
                pc = NNN + V[0x0];
                break;
            case 0xC000:
#if defined(THREEDO) || defined(PCFX)
		srand(0);
#else
                srand(time(NULL));
#endif
                V[X] = R & KK;
                break;
            case 0xD000:
                /* Das drawing! */
                V[0xF] = 0;
                px = py = 0;
                for (y=0; y < N; y++) 
                {
                    py = ((V[Y] + y) % 32);
                    for (x=0; x < 8; x++) 
                    {
                        px = ((V[X] + x) % 64);
                        color = (memory[I + y] >> (7 - x)) & 1;
                        ccolor = gfx[py][px];
                        if (color && ccolor) V[0xF] = 1;
                        color ^= ccolor;
                        gfx[py][px] = color;
                        bounds.x = DX;
                        bounds.y = DY;
                        bounds.w = SCALE;
                        bounds.h = SCALE;
                        draw_pixel(bounds.x, bounds.y);
                    }
                }
                
                update_screen();
                
                break;
            case 0xE000:
                switch (KK) {
                    case 0x009E:
                        if (keys[V[X]] == 1)
                            pc += 2;
                        break;
                    case 0x00A1:
                        if (keys[V[X]] == 0)
                            pc += 2;
                        break;
                }
                break;
            case 0xF000:
                switch (KK) {
                    case 0x0007:
                        V[X] = DT;
                        break;
                    case 0x000A:
                        temp = -1;
                        for (i=0; i<16; i++) {
                            if (keys[i]) {
                                temp = i;
                                break;
                            }
                        }
                        if (temp != -1)
                            V[X] = temp;
                        else
                            pc -= 2;
                        break;
                    case 0x0015:
                        DT = V[X];
                        break;
                    case 0x0018:
                        ST = V[X];
                        break;
                    case 0x001E:
                        I += V[X];
                        break;
                    case 0x0029:
                        I = V[X] * 5;
                        break;
                    case 0x0033:
                        memory[I] = V[X] / 100;
                        memory[I + 1] = (V[X] / 10) % 10;
                        memory[I + 2] = V[X] % 10;
                        break;
                    case 0x0055:
                        for (i=0; i<=X; i++)
                            memory[I + i] = V[i];
                        break;
                    case 0x0065:
                        for (i=0; i<=X; i++)
                            V[i] = memory[I + i];
                        break;
                    default:
                        printf("Unknown opcode: 0x%X\n", opcode);
                        break;
                }
                break;
            default:
                printf("Unknown opcode: 0x%X\n", opcode);
                break;
        }
			if (DT > 0) 
			{
				DT--;
			}
			if (ST > 0) 
			{
				ST--;
				if (!ST) beep();
			}
        }
    }
    
#if defined(THREEDO) 
#elif defined(PCFX)
#else
    SDL_Quit();
#endif
    
}


void draw_pixel(int x, int y)
{
#ifdef THREEDO
	fill_rectangle(x, y+60, SCALE, 0, (color ? 255 : 0), 0);
#elif defined(PCFX)
	print("O", x ,y , 0);
#else
	
	int color_draw;
	SDL_Rect position;
	
	position.x = x;
	position.y = y;
	position.w = SCALE;
	position.h = SCALE;
	
	if (color)
	{
		color_draw = SDL_MapRGB(screen->format, 0, 255, 0);
	}
	else
	{
		color_draw = SDL_MapRGB(screen->format, 0, 0, 0);
	}
	
	SDL_FillRect(screen, &position, color_draw);
	
#endif
	
}

void Controls()
{

#ifdef THREEDO
	uint32	gButtons;
	DoControlPad(1, &gButtons, (ControlUp | ControlDown | ControlLeft | ControlRight));
	
	
	if (gButtons & ControlLeft)	
	{
		keys[4] = 1;
	}
	else
	{
		keys[4] = 0;
	}
	
	if (gButtons & ControlRight)	
	{
		keys[6] = 1;
	}
	else
	{
		keys[6] = 0;
	}
	
	if (gButtons & ControlC)	
	{
		keys[12] = 1;
	}
	else
	{
		keys[12] = 0;
	}

	if (gButtons & ControlA)	
	{
		keys[5] = 1;
	}
	else
	{
		keys[5] = 0;
	}

#elif defined(PCFX)

	Controls();

	if (JOYPAD_LEFT)	
	{
		keys[4] = 1;
	}
	else
	{
		keys[4] = 0;
	}
	
	if (JOYPAD_RIGHT)	
	{
		keys[6] = 1;
	}
	else
	{
		keys[6] = 0;
	}
	
	if (JOYPAD_C)	
	{
		keys[12] = 1;
	}
	else
	{
		keys[12] = 0;
	}

	if (JOYPAD_A)	
	{
		keys[5] = 1;
	}
	else
	{
		keys[5] = 0;
	}
	
#else

        if (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case (SDL_QUIT):
                    running = 0;
                    break;
                case (SDL_KEYDOWN):
                    key = SDL_GetKeyName(event.key.keysym.sym);
                    if (strlen(key) > 1) break;
                    chr = key[0];
                    switch(chr) {
                        case KEY_1:
                            keys[0] = 1;
                            break;
                        case KEY_2:
                            keys[1] = 1;
                            break;
                        case KEY_3:
                            keys[2] = 1;
                            break;
                        case KEY_C:
                            keys[3] = 1;
                            break;
                            
                        case KEY_4:
                            keys[4] = 1;
                            break;
                        case KEY_5:
                            keys[5] = 1;
                            break;
                        case KEY_6:
                            keys[6] = 1;
                            break;
                        case KEY_D:
                            keys[7] = 1;
                            break;
                            
                        case KEY_7:
                            keys[8] = 1;
                            break;
                        case KEY_8:
                            keys[9] = 1;
                            break;
                        case KEY_9:
                            keys[10] = 1;
                            break;
                        case KEY_E:
                            keys[11] = 1;
                            break;
                            
                        case KEY_A:
                            keys[12] = 1;
                            break;
                        case KEY_0:
                            keys[13] = 1;
                            break;
                        case KEY_B:
                            keys[14] = 1;
                            break;
                        case KEY_F:
                            keys[15] = 1;
                            break;
                        
                        case KEY_NEXT_OPCODE:
                            next_opcode = 1;
                    }
                    break;
                case (SDL_KEYUP):
                    key = SDL_GetKeyName(event.key.keysym.sym);
                    if (strlen(key) > 1) break;
                    chr = key[0];
                    switch(chr) {
                        case KEY_1:
                            keys[0] = 0;
                            break;
                        case KEY_2:
                            keys[1] = 0;
                            break;
                        case KEY_3:
                            keys[2] = 0;
                            break;
                        case KEY_C:
                            keys[3] = 0;
                            break;
                            
                        case KEY_4:
                            keys[4] = 0;
                            break;
                        case KEY_5:
                            keys[5] = 0;
                            break;
                        case KEY_6:
                            keys[6] = 0;
                            break;
                        case KEY_D:
                            keys[7] = 0;
                            break;
                            
                        case KEY_7:
                            keys[8] = 0;
                            break;
                        case KEY_8:
                            keys[9] = 0;
                            break;
                        case KEY_9:
                            keys[10] = 0;
                            break;
                        case KEY_E:
                            keys[11] = 0;
                            break;
                            
                        case KEY_A:
                            keys[12] = 0;
                            break;
                        case KEY_0:
                            keys[13] = 0;
                            break;
                        case KEY_B:
                            keys[14] = 0;
                            break;
                        case KEY_F:
                            keys[15] = 0;
                            break;
                        case KEY_PAUSE:
                            /* Pause */
                            paused = !paused;
                            if (paused) {
                                SDL_WM_SetCaption(
                                    "CHIP8 emulator - paused", 0);
                                cinst = 0;
                            }
                            else {
                                SDL_WM_SetCaption(
                                    "CHIP8 emulator - running", 0);
                                cinst = INST_PER_CYCLE;
                            }
                            break;
                        case KEY_NEXT_OPCODE:
                            if (paused) 
                            {
                                next_opcode = 0;
                            }
                            break;
                        case KEY_INFO:
                            /* Print info */
                            for (temp=0; temp<16; temp++) {
                                printf("V%-2d ", temp);
                            }
                            printf("\n");
                            for (temp=0; temp<16; temp++) {
                                printf("%-3d ", V[temp]);
                            }
                            printf("\n");
                            printf("I: %d\n", I);
                            printf("pc: %d\n", pc);
                            printf("stack pointer: %d\n", sp);
                            printf("Stimer: %d\n", ST);
                            printf("Dtimer: %d\n", DT);
                            break;
                        case KEY_HALT:
                            /* Halt */
                            running = 0;
                            break;
                    }
                    break;
            }
            /*if (!running) break;*/
        }
     
#endif
       
}

void update_screen()
{
#ifdef THREEDO
	affichageMiseAJour();
	affichageRendu();
#elif defined(PCFX)
	
#else
	SDL_Flip(screen);
#endif
}

void screen_init()
{
#ifdef THREEDO
	unsigned char i;
	affichageInitialisation();
	InitializeControlPads();
	imageFondChargement("System/back.cel");
	affichageMiseAJour();
	affichageRendu();
	
	/* Leave some time for the 3DO to upload the background into VRAM*/
	for (i=0;i<50;i++)
	{
		affichageRendu();
	}
#elif defined(PCFX)
	crapload();
	clear_bg0();
	set_to_text_mode();
	Init_controls();
#else
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(WIDTH*SCALE, HEIGHT*SCALE, 0, SDL_SWSURFACE);
#endif	
}
