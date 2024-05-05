#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"

typedef enum
{
    PT_Point,
    PT_Shape,
    PT_Sprite,
    PT_MAX
}ParticleType;

typedef struct
{
    Uint32 ttl;
    ParticleType type;

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    Color color;
    Color colorVelocity;
    Color colorAcceleration;

    Shape shape;
    Sprite *sprite;
    float frame;
    float frameRate;
    int frameStart;
    int frameEnd;
    int loops; //if > 1, loop, else, stop on last frame
}Particle;

/**
 * @brief start particle system and allocate space for all particles
 * @param particleMax the number of particles the game has access to
*/
void particle_init(Uint32 particleMax);

/**
 * @brief clear all active particles
*/
void particle_clear();

/**
 * @brief make a particle
 * @return reference to particle
*/
Particle *particle_new();

/**
 * @brief draw all active particles
*/
void particle_system_draw();

/**
 * @brief spray particles in a certain direction from a certain position;
 * @param count the number of particles
 * @param ttl the lifetime of the particles
 * @param color color of particles
 * @param variation the color to vary the other color with
 * @param dir the direction of the particles
 * @param speed the speed of the particles
 * @param spread the spread from dir
 * @param pos the starting point of the particles
*/
void particle_spray(Uint32 count, Uint32 ttl, Color color, Color variation, Vector2D dir, float speed, float spread, Vector2D pos);
#endif