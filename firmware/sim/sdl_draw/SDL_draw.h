/*!
  \file SDL_draw.h
  \author Mario Palomo Torrero <mpalomo@ihman.com>
  \author Jose M. de la Huerga Fernández
  \author Pepe González Mora
  \date 05-2002

  Drawing primitives for SDL. Main header file.

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
#ifndef SDL_DRAW_H
#define SDL_DRAW_H

#include "SDL/SDL.h"
#include "SDL/begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif


extern DECLSPEC
void (*Draw_FillCircle)(SDL_Surface *super,
                        Sint16 x0, Sint16 y0, Uint16 r,
                        Uint32 color);


/* We'll use SDL for reporting errors */
#define Draw_SetError  SDL_SetError
#define Draw_GetError  SDL_GetError


#ifdef __cplusplus
} /* extern "C" */
#endif

#include "SDL/close_code.h"

#endif /* SDL_DRAW_H */

