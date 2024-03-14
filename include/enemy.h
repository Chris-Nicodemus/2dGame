#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

typedef enum
{
    NotEnemy,
    Bug,
    Goblin,
    Dragon,
    Knight,
    Mech
} EnemyType;

typedef enum
{
    EnemyIdle,
    EnemyAttack,
    EnemyDie
} EnemyState;
/**
 * @brief starts the entity system
*/
void enemy_system_init(Uint8 enemyMax);

/**
 * @brief creates an enemy entity
 * @param pos the position for the enemy to spawn
 * @param type the type of enemy to spawn
*/
Entity *enemy_new(Vector2D pos, EnemyType type);

/**
 * @brief determine the intent of all living enemies
*/
void enemy_intent();

/**
 * @brief all enemies execute their intents
*/
void enemy_act();

/**
 * @brief deal damage to an enemy
*/
void enemy_damage(int damage, DamageType damageType);
#endif