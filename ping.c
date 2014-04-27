#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>

#define W 200
#define H 200

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
signed int b1[W*H],b2[W*H];

signed int * getcell(int x, int y, int * from)
{
	if (x<0 || y<0 || x>W || y>H) return -1;
	return from[x+y*W];
}
void setcell(int x, int y, signed int v, int * in)
{
	if (x<0 || y<0 || x>W || y>H) return;
	in[x+y*W]=v;
}
void propagate(int x, int y, signed int * now, signed int * then)
{
	signed int v=0;
	if (getcell(x,y,then)==-1)
	{
		setcell(x,y,-1,now);
	}
	else
	{
		v=MAX(v,getcell(x-1,y,then));
		v=MAX(v,getcell(x,y-1,then));
		v=MAX(v,getcell(x+1,y,then));
		v=MAX(v,getcell(x,y+1,then));
		if (v>0) v--;
		setcell(x,y,v,now);
	}
}

int main(int argv, char ** argc)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(W,H,32,0);
	signed int * now, * then;
	int x,y;
	now=b1;
	then=b2;
	for (x=0;x<W;x++)
	{
		for (y=0;y<H;y++)
		{
			setcell(x,y,0,then);
		}
	}
	for (x=10;x<50;x++)
	{
		setcell(x,y,-1,then);
	}
	setcell(70,70,200,then);

}

