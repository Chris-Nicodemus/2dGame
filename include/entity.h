#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"


typedef enum
{
    Combat,
    Event,
    Choice,
    Map,
    MainMenu
} State;

typedef enum
{
    Player,
    ChoiceIcon,
    MapIcon,
    EventIcon,
    CombatButton,
    Card,
    Enemy
} EntType;

typedef enum
{
    None,
    Explore,
    Shop,
    Shrine,
    Chest
} EventType;

typedef enum
{
    Basic,
    Poison
} DamageType;

typedef struct Entity_S
{
    Uint8  _inuse;
    Sprite *sprite;
    float frame; //sprite frame to render but needs to be cast as a Uint32
    Uint32 frameStart; //frame starting point
    Uint32 frameEnd;   //frame end point
    int state;         //determines what animation to be doing
    Vector2D flip;   //tells renderer to flip image if necessary
    Vector2D position;    
    Vector2D scale;
    Vector2D pixel;     //size of image in terms of pixels
    float drawOffsetX; //how much to offset draw and bounds. Make negative to move left
    float drawOffsetY; //how much to offset draw and bounds. Make negative to move up
    struct Entity_S *owner; //for cards to have a reference to their owner and enemies to have a reference to the player
    Bool gift;          //true if the card is supposed to be able to be claimed by the player
    Bool noHighlight;   //true if you do not want entity to get highlighted on mouse
    EntType type;       //type of entity
    int iconType;       //if icon, type of icon
    int enemyType;      //if enemy, type of enemy
    int action;         //the action an enemy will do
    int actionStrength; //the number associated with that action. (amount of damage or block to gain etc)
    int actionStrength2;//if action has two things happen, this is the strength of the second action
    

    

    int health;
    int healthMax;
    int block;
    int energy;
    int energyMax;
    int overload;
    int gold;           //for players and enemies
    Bool airborne;      //state for player
    
    void (*think) (struct Entity_S *self);
    void (*update) (struct Entity_S *self);
    void (*free) (struct Entity_S *self);
    void (*leftClick) (struct Entity_S *self);
    void (*rightClick) (struct Entity_S *self);
    void (*draw) (struct Entity_S *self);   //additional stuff to draw other than the sprite itself (like health bars for example)
    void *data;

    Rect bounds; //size of sprite
    Bool mouse; //true if mouse is held over bounds
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
 * @brief draw all active entities
*/
void entity_system_draw();

/**
 * highlight all moused entities
*/
void entity_highlight_all();

/**
 * @brief get a reference to the player
 * @return the player
*/
Entity *entity_get_player();
#endif