#ifndef __CONSUMABLE_H__
#define __CONSUMABLE_H__

#include "entity.h"
//struct for drawing gold
typedef struct
{
    Sprite *sprite;
    Vector2D offset;
    Vector2D scale;
} Gold;

/**
 * @brief Set up some stuff for consumables at start of game
*/
void consumable_init();

/**
 * @brief make a consumable entity
 * @param pos the position of the consumable
 * @param name the type of consumable to be
 * @param owned true if the player owns it, false if in a shop
*/
Entity *consumable_new(Vector2D pos, char *name, Bool owned);

/**
 * @brief change a consumable's name and sprite
 * @param self the consumable in question
 * @param name the type of consumable to convert to
*/
void consumable_set(Entity *self, char *name);

/**
 * @brief draw the object's cost in the shop along with a little cute gold symbol
 * @param self the object to have its cost drawn on screen
*/
void consumable_draw_gold(Entity *self);

#endif