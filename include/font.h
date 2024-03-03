#ifndef __FONT_H__
#define __FONT_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "gfc_text.h"
#include "gfc_color.h"

typedef enum
{
    FS_Small,
    FS_Medium,
    FS_Large,
    FS_MAX
}FontStyles;


/**
 * @brief start font rendering system that closes on exit
*/
void font_init();

/**
 * @brief draw text specified color, size, and position
 * @param text the text to draw
 * @param style the font to draw
 * @param color the color to draw the text in
 * @param position where the text will be placed
 * @param scale (optional) how large the text will be. Set to (0,0) if default
*/
void font_draw_text(const char* text, FontStyles style, Color color, Vector2D position, Vector2D scale);
#endif