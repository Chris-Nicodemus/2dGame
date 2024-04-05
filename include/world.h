#ifndef __WORLD_H__
#define __WORLD_H__

#include "gfc_list.h"
#include "gf2d_sprite.h"


typedef struct 
{
    Sprite *background;
    Vector2D scale;
    char    *name;
    List    *bgs;
    List    *icons;     //List of entities for permanent icons
} World;


/**
 * @brief create a world object
 * @return a pointer to the object or NULL on error
*/
World *world_new();

/**
 * @brief free the world
 * @param world the world to be freed
*/
void world_free(World *world);

/**
 * @brief draw the world
 * @param world the world to draw
*/
void world_draw(World *world);

/**
 * @brief open the mainMenu. May need to be called from several other files
*/
void world_open_main_menu();
#endif