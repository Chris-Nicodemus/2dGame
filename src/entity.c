#include "simple_logger.h"

#include "entity.h"


typedef struct 
{
    Entity *entity_list;
    Uint32 entity_max;
} EntityManager;


static EntityManager entity_manager = {0}; /**<initialize a global entity manager>*/


void entity_system_close()
{
    entity_clear_all(NULL);

    if(entity_manager.entity_list)
    free(entity_manager.entity_list);

    memset(&entity_manager,0,sizeof(EntityManager));

    slog("Entity System Close");
}


void entity_system_init(Uint32 maxEnts)
{
    if(entity_manager.entity_list)
    {
        slog("cannot have more than one entity manager");
        return;
    }
    
    if(!maxEnts)
    {
        slog("cannor allocate 0 entities!");
        return;
    }

    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),maxEnts);
    if(!entity_manager.entity_list)
    {
        slog("failed to allocate global entity list");
        return;
    }

    entity_manager.entity_max = maxEnts;
    slog("entity system start");
    atexit(entity_system_close);
}



void entity_clear_all(Entity *ignore)
{
    int i;
    for(i = 0; i < entity_manager.entity_max; i++)
    {
        //skip inactive
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        //skip ignore
        if(&entity_manager.entity_list[i] == ignore)
        continue;

        //then free
        entity_free(&entity_manager.entity_list[i]);
    }
}



Entity *entity_new()
{
    int i;

    slog("new ent called");

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(entity_manager.entity_list[i]._inuse)
        continue;

        memset(&entity_manager.entity_list[i],0,sizeof(Entity));
        entity_manager.entity_list[i]._inuse = 1;
        //do all stuff that all entities want here

        slog("entity made");
        //return the reference to entity
        return &entity_manager.entity_list[i];
    }
    slog("No more available entities");
    return NULL;
}


void entity_free(Entity *self)
{
    if(!self)
    {
        slog("entity free was given NULL");
        return;
    }

    gf2d_sprite_free(self->sprite);
    //anything else allocated for the entity would get cleaned up here
    if(self->free)
    self->free(self->data);
}


void entity_think(Entity *self)
{
    if(!self)
    return;
    //any stuff that all entities should do goes here

    if(self->think)
    self->think(self);
}


void entity_system_think()
{
    int i;

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        entity_think(&entity_manager.entity_list[i]);
    }
}


void entity_update(Entity *self)
{
    if(!self)
    return;
    //any stuff that all entities should do goes here

    if(self->update)
    self->update(self);
}


void entity_system_update()
{
int i;

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        entity_update(&entity_manager.entity_list[i]);
    }
}


void entity_draw(Entity *self)
{
    if(!self)
    return;

    
    if(self->sprite)
    {
        gf2d_sprite_render(self->sprite,self->position,&self->scale,NULL,NULL,&self->flip,NULL,NULL,(Uint32)self->frame);
    }
}


void entity_system_draw()
{
    int i;

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        entity_draw(&entity_manager.entity_list[i]);
    }
}