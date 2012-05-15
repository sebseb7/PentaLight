/*!
  \file SDL_draw.c
  \author Mario Palomo Torrero <mpalomo@ihman.com>
  \author Jose M. de la Huerga Fernández
  \author Pepe González Mora
  \date 05-2002

  Drawing primitives for SDL. Main implementation file.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "SDL_draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>  /*wmemset*/


/* Draw_Init is defined at the end */
static void Draw_Init(void);




/*================= BEGIN of Draw_FillCircle ===================*/

#define SDL_DRAW_BPP 1
#define SDL_DRAWFUNCTION  Draw_FillCircle_1
#include "Draw_FillCircle.c"
#undef SDL_DRAWFUNCTION
#undef SDL_DRAW_BPP

#define SDL_DRAW_BPP 2
#define SDL_DRAWFUNCTION  Draw_FillCircle_2
#include "Draw_FillCircle.c"
#undef SDL_DRAWFUNCTION
#undef SDL_DRAW_BPP

#define SDL_DRAW_BPP 3
#define SDL_DRAWFUNCTION  Draw_FillCircle_3
#include "Draw_FillCircle.c"
#undef SDL_DRAWFUNCTION
#undef SDL_DRAW_BPP

#define SDL_DRAW_BPP 4
#define SDL_DRAWFUNCTION  Draw_FillCircle_4
#include "Draw_FillCircle.c"
#undef SDL_DRAWFUNCTION
#undef SDL_DRAW_BPP

static
void Draw_FillCircle_Init(SDL_Surface *super,
                          Sint16 x0, Sint16 y0, Uint16 r,
                          Uint32 color)
{
  Draw_Init();
  Draw_FillCircle(super, x0, y0, r, color);
}


void (*Draw_FillCircle)(SDL_Surface *super,
                        Sint16 x0, Sint16 y0, Uint16 r,
                        Uint32 color) = Draw_FillCircle_Init;

/*================== END of Draw_FillCircle ====================*/



/*Assignment of function pointers:*/
#define SDL_DRAW_FUNCTIONS_BPP(x) \
      Draw_FillCircle  = Draw_FillCircle_##x;  \

static
void Draw_Init(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  if (!screen) {
    fprintf(stderr,"SDL_draw: SDL_Draw_Init ERROR!!."
                  " Video Surface not found\n");
    exit(-2);
  }

  switch(screen->format->BytesPerPixel) {
    case 1:
      SDL_DRAW_FUNCTIONS_BPP(1);
    break;

    case 2:
      SDL_DRAW_FUNCTIONS_BPP(2);
    break;

    case 3:
      SDL_DRAW_FUNCTIONS_BPP(3);
    break;

    case 4:
      SDL_DRAW_FUNCTIONS_BPP(4);
    break;
  }/*switch*/

}/*Draw_Init*/

#undef SDL_DRAW_FUNCTIONS_BPP


