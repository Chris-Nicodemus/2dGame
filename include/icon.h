#ifndef __ICON_H__
#define __ICON_H__

#include "entity.h"

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
    ChoiceShrine
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


//data will contain the icon type from the enumeration
#endif