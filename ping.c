#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define W 400
#define H 400

signed int b1[W*H],b2[W*H];

signed int  getcell(int x, int y, int * from)
{
	if (x<0 || y<0 || x>=W || y>=H) return -1;
	return from[x+y*W];
}
void setcell(int x, int y, signed int v, int * in)
{
	if (x<0 || y<0 || x>=W || y>=H) return;
	in[x+y*W]=v;
}
signed int MAX(signed int a, signed int b)
{
	if (a>b) return a; else return b;
}
void propagate(int x, int y, signed int * now, signed int * then, int flag)
{
	signed int v=getcell(x,y,then);
	if (v==-1)
	{
		setcell(x,y,-1,now);
	}
	else if (v==-3)
	{
		setcell(x,y,0,now);
	}
	else if (v==-2)
	{
		setcell(x,y,0,now);
	}
	else if (v>0)
	{
		setcell(x,y,-2,now);
	}
	else
	{
		v=MAX(v,getcell(x-1,y,then));
		v=MAX(v,getcell(x,y-1,then));
		v=MAX(v,getcell(x+1,y,then));
		v=MAX(v,getcell(x,y+1,then));
		if (flag)
		{
			v=MAX(v,getcell(x-1,y-1,then));
			v=MAX(v,getcell(x+1,y-1,then));
			v=MAX(v,getcell(x+1,y+1,then));
			v=MAX(v,getcell(x-1,y+1,then));
		}
		if (v>0) v--;
		//if (v==0 && rand()%16384==0) v=200;
		setcell(x,y,v,now);
	}
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}



int main(int argv, char ** argc)
{
	SDL_Surface * screen;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Event event;
	Uint32 color;
	screen = SDL_SetVideoMode(W,H,32,SDL_SWSURFACE | SDL_DOUBLEBUF);
	signed int * now, * then, * carry;
	int x,y,v;
	float s2=sqrt(2);
	float count;
	now=b1;
	then=b2;
	int flag=0;
	srand(1);
	for (x=0;x<W;x++)
	{
		for (y=0;y<H;y++)
		{
			setcell(x,y,0,then);
		}
	}
	for (x=40;x<160;x++)
	{
		for (y=50;y<190;y++)
		{
			setcell(x,y,-1,then);
		}
	}
	while(1)
	{
		count+=s2;
		if (count>4)
		{
			count-=4;
			flag=1;
		}
		else
			flag=0;
		for (y=0;y<H;y++)
			for(x=0;x<W;x++)
				propagate(x,y,now,then,flag);
		if (SDL_MUSTLOCK(screen))
			SDL_LockSurface(screen);
		for (y=0;y<H;y++)
		{
			for (x=0;x<W;x++)
			{
				v=getcell(x,y,now);
				if (v==-1) 
					color=SDL_MapRGB(screen->format,0,0,0);
				else if (v==-2)
					color=SDL_MapRGB(screen->format,0,0,0);
				else if (v==-3)
					color=SDL_MapRGB(screen->format,0,0,0);
				else
					color=SDL_MapRGB(screen->format,0,0,v);
				putpixel(screen,x,y,color);
			}
		}
		if (SDL_MUSTLOCK(screen))
			SDL_UnlockSurface(screen);
		SDL_UpdateRect(screen,0,0,W,H);
		carry=now;
		now=then;
		then=carry;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
					setcell(event.button.x,event.button.y,200,then);
					break;
				case SDL_QUIT:
					return 0;
			}
		}
	}
}

