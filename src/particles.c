#include "simple_logger.h"
#include "particles.h"
#include "gf2d_draw.h"

typedef struct
{
    Particle *particleList;
    Uint32    particleMax;
}ParticleManager;

ParticleManager particleManager = {0};

void particle_close()
{
    if(particleManager.particleList)
    {
        free(particleManager.particleList);
    }

    memset(&particleManager,0,sizeof(ParticleManager));
}

void particle_init(Uint32 particleMax)
{
    if(particleManager.particleList)
    {
        slog("Cannot start particle system multiple times");
        return;
    }

    if(!particleMax)
    {
        slog("Cannot init particle system with Max 0");
        return;
    }


    particleManager.particleList = gfc_allocate_array(sizeof(Particle),particleMax);
    if(!particleManager.particleList)
    {
        slog("Particle system start fail");
        return;
    }

    particleManager.particleMax = particleMax;
    atexit(particle_close);
}

/**
 * @brief clear all active particles
*/
void particle_clear()
{
    int i;
    if(!particleManager.particleList) return;

    for(i = 0; i < particleManager.particleMax; i++)
    {
        if(particleManager.particleList[i].sprite)
                gf2d_sprite_free(particleManager.particleList[i].sprite);

        
    }
    memset(particleManager.particleList, 0, sizeof(Particle) * particleManager.particleMax);
}

/**
 * @brief make a particle
 * @return reference to particle
*/
Particle *particle_new()
{
    int i;
    for(i = 0; i < particleManager.particleMax; i++)
    {
        if(particleManager.particleList[i].ttl == 0)
        {
            if(particleManager.particleList[i].sprite)
                gf2d_sprite_free(particleManager.particleList[i].sprite);

            memset(&particleManager.particleList[i], 0, sizeof(Particle));
            particleManager.particleList[i].ttl = 1;
            return &particleManager.particleList[i];
        }
    }

    return NULL; //if full return nothing
}

void particle_draw(Particle *p)
{
    if(!p) return;

    switch(p->type)
    {
        case PT_MAX:
        case PT_Point:
            gf2d_draw_pixel(p->position,p->color);
            break;
        case PT_Shape:
            gf2d_draw_shape(p->shape,p->color,p->position);
            break;
        case PT_Sprite:
            gf2d_sprite_draw(p->sprite,p->position,NULL,NULL,NULL,NULL, &p->color, (Uint32)p->frame);
        default:
            slog("particle type error");
            return;
    }
}

void particle_update(Particle *p)
{
    if(!p) return;

    p->ttl--;
    if(!p->ttl) return;

    vector2d_add(p->position,p->position,p->velocity);
    vector2d_add(p->velocity,p->velocity,p->acceleration);

    gfc_color_add(&p->color,p->color,p->colorVelocity);
    gfc_color_add(&p->colorVelocity,p->colorVelocity,p->colorAcceleration);

    if(p->type == PT_Sprite)
    {
        p->frame += p->frameRate;

        if(p->frame > p->frameEnd)
        {
            if(p->loops)
                p->frame = p->frameStart;
            else
            {
                p->frame = p->frameEnd;
                p->frameRate = 0;
            }
        }
    }
}

void particle_system_draw()
{
    int i;
    for(i = 0; i < particleManager.particleMax; i++)
    {
        if(particleManager.particleList[i].ttl)
        {
            particle_update(&particleManager.particleList[i]);
            particle_draw(&particleManager.particleList[i]);
        }
    }
}

void particle_spray(Uint32 count, Uint32 ttl, Color color, Color variation, Vector2D dir, float speed, float spread, Vector2D pos)
{
    int i;
    Particle *p;
    Vector2D direction;
    vector2d_normalize(&dir);

    for(i = 0; i < count; i++)
    {
        p = particle_new();
        if(!p) return;

        p->color = color;

        p->color.r += gfc_crandom() * variation.r;
        p->color.g += gfc_crandom() * variation.g;
        p->color.b += gfc_crandom() * variation.b;
        p->color.a += gfc_crandom() * variation.a;

        direction = vector2d_rotate(dir,gfc_crandom()*spread);
        vector2d_scale(p->velocity,direction,speed);

        p->ttl = ttl;
        p->position = pos;
    }
}