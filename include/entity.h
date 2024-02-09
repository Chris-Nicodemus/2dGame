#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"


typedef struct Entity_S
{
    Uint8  _inuse;
    Sprite *sprite;
    float frame; //sprite frame to render but needs to be cast as a Uint32
    Vector2D position;    
    void (*think) (struct Entity_S *self);
    void (*update) (struct Entity_S *self);
    void (*free) (struct Entity_S *self);
    void *data;
}Entity;

/**
 * @brief create entity system and resource manager
 * @param maxEnts the maximum amount of entities allowed at any one time
*/
void entity_system_init(Uint32 maxEnts);

/**
 * @brief clears all entities, or all entites except 1
 * @param ignore the entity that will not be cleared. Null if clear all.
*/
void entity_clear_all(Entity *ignore);

/**
 * @brief returns a blank entity
 * @return a pointer to a blank entity. Null if over entity limit
*/
Entity *entity_new();

/**
 * @brief clean up all data and frees space for use
 * @param self Entity to free
*/
void entity_free(Entity *self);

/**
 * @brief run think functions for all active entities
*/
void entity_system_think();

/**
 * @brief run update functions for all active entities
*/
void entity_system_update();

/**
 * @brief draw all activ entities
*/
void entity_system_draw();
#endif