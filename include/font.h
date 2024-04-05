#ifndef __FONT_H__
#define __FONT_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "gfc_text.h"
#include "gfc_color.h"
#include "gfc_shape.h"

typedef enum
{
    FS_Small,
    FS_Medium,
    FS_Large,
    FS_MAX
}FontStyles;

typedef enum
{
    TT_MainMenu,
    TT_MainMenu_Button
} TextType;

typedef struct Text_S {
Uint8 _inuse;
char *text;
FontStyles style;
TextType type; //determines what methods to use
Bool mouse;
Vector2D scale;
Vector2D position;
Vector2D textSize;
Uint32 ttl; //time that text will be displayed on screen, 0 if forever
Color color;
Rect bounds;



void (*update) (struct Text_S *self);
//void (*free) (struct Text_S *self);
void (*leftClick) (struct Text_S *self);
void (*rightClick) (struct Text_S *self);
void (*moused) (struct Text_S *self); //triggers when moused over
} Text;

/**
 * @brief start font rendering system that closes on exit
*/
void font_init(Uint32 maxText);

/**
 * @brief draw text specified color, size, and position
 * @param text the text to draw
 * @param style the font to draw
 * @param color the color to draw the text in
 * @param position where the text will be placed
 * @param scale (optional) how large the text will be. Set to (0,0) if default
*/
void font_draw_text(const char* text, FontStyles style, Color color, Vector2D position, Vector2D scale);

/**
 * @brief returns a text object
 * @return a pointer to a text object. Null if over text limit
*/
Text *text_new(char *text, FontStyles style, Vector2D position, Vector2D scale, Color color, Uint32 ttl, TextType type);

/**
 * @brief frees the text object
 * @param self the text in question
*/
void text_free(Text *self);

/**
 * @brief free all text objects
 * @param ignore text object to spare. Null if you want to clear everything
*/
void text_clear_all(Text *ignore);

/**
 * @brief clear all text that is permanently on screen
 * @param ignore text object to spare
*/
void text_clear_perms(Text *ignore);

/**
 * @brief update all text objects that have an update function
*/
void text_update_all();

/**
 * @brief draw all text objects
*/
void text_draw_all();
#endif