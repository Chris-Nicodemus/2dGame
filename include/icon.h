#ifndef __ICON_H__
#define __ICON_H__

#include "entity.h"
#include "world.h"

typedef enum
{
    Question,
    MapBattle,
    MapExplore,
    MapShop,
    MapShrine,
    ChoiceBattle,
    ChoiceExplore,
    ChoiceShop,
    ChoiceShrine,
    EventShrine,
    EventChest,
    EndTurn
} Icons;

/**
 * @brief start icon system
*/

void icon_init();
/**
 * @brief spawn an icon
 * @return pointer to icon, or NULL on error
 * @param  pos position that the icon will spawn
*/
Entity *icon_new(Vector2D pos, Icons icon);

/**
 * @brief set entity icon to a specific sprite
 * @param self icon in question
 * @param icon the sprite to associate the icon with
*/
void icon_set_sprite(Entity *self, Icons icon);

/**
 * @brief give the icon file a reference to the world
 * @param w the world in question
*/
void icon_get_world(World *w);

/**
 * @brief give the icon file a reference to the player
 * @param p the player in question
*/
void icon_get_player(Entity *p);
//data will contain the icon type from the enumeration

/**
 * @brief closes the current event
*/
void event_close();

/**
 * @brief get the text releted for the event
 * @return the text to get 
*/
char *icon_get_event_text();
#endif