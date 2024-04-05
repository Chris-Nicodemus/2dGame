#include "simple_logger.h"

#include "gf2d_draw.h"
#include "entity.h"


typedef struct 
{
    Entity *entity_list;
    Uint32 entity_max;
} EntityManager;


static EntityManager entity_manager = {0}; /**<initialize a global entity manager>*/
extern Bool leftClicked;
extern Bool rightClicked;
extern State state;

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

    //slog("new ent called");

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(entity_manager.entity_list[i]._inuse)
        continue;

        memset(&entity_manager.entity_list[i],0,sizeof(Entity));
        entity_manager.entity_list[i]._inuse = 1;
        //do all stuff that all entities want here

        //slog("entity made");
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

    self->_inuse = false;
    //anything else allocated for the entity would get cleaned up here
    if(self->free)
    self->free(self);

}

Bool entity_active(Entity *self)
{
    EntType type;

    if(!self) return false;

    type = self->type;

    if(type == Card) return true;
    
    switch(state)
    {
        case Combat:
            if(type == Player || type == Enemy || type == CombatButton)
                return true;
            else
                return false;
        case Event:
            if(type == Player || type == EventIcon)
                return true;
            else
                return false;
        case Choice:
            if(type == ChoiceIcon)
                return true;
            else
                return false;
        case Map:
            if(type == MapIcon)
                return true;
            else
                return false;
    }
    return false;
}

void entity_think(Entity *self)
{
    int mx,my;
    Uint32 mButton;

    if(!self)
    return;

    //any stuff that all entities should do goes here
    
    self->mouse = false;

    if(!entity_active(self))
    return;

    self->bounds = gfc_rect(self->position.x + self->drawOffsetX,self->position.y + self->drawOffsetY,self->scale.x * self->pixel.x,self->scale.y * self->pixel.y);

    mButton = SDL_GetMouseState(&mx,&my);
    
    if(gfc_point_in_rect(vector2d(mx,my),self->bounds))
    {
        self->mouse = true;
    }
    
    if(self->mouse && mButton == SDL_BUTTON_LEFT && !leftClicked)
    {
        leftClicked = true;
        if(self->leftClick)
        self->leftClick(self);
    }

    if(self->mouse && mButton == 4 && !rightClicked)
    {
        //slog("happening");
        rightClicked = true;
        if(self->rightClick)
        self->rightClick(self);
    }

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
    if(self->frameEnd)
    {
        self->frame = self->frame + 0.1;
        if(self->frame >= self->frameEnd)
        {
            self->frame = self->frameStart;
        }
    }
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

    Vector2D pos = self->position;
    pos.x = pos.x + self->drawOffsetX;
    pos.y = pos.y + self->drawOffsetY;
    
    if(self->sprite)
    {
        gf2d_sprite_render(self->sprite,pos,&self->scale,NULL,NULL,&self->flip,NULL,NULL,(Uint32)self->frame);
    }

    if(self->draw)
    {
        self->draw(self);
    }
}


void entity_system_draw()
{
    int i;

    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        if(!entity_active(&entity_manager.entity_list[i])) continue;

        entity_draw(&entity_manager.entity_list[i]);
        //slog("%i",i);
    }
}


void entity_highlight(Entity *self)
{
    gf2d_draw_rect(self->bounds,gfc_color(1,1,1,1));
}


void entity_highlight_all()
{
    int i;
    for(i = 0; i < entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        if(!entity_active(&entity_manager.entity_list[i])) continue;
        
        if(entity_manager.entity_list[i].mouse && !entity_manager.entity_list[i].noHighlight)
        entity_highlight(&entity_manager.entity_list[i]);
    }

}

Entity *entity_get_player()
{
    int i;

    for(i = 0; entity_manager.entity_max; i++)
    {
        if(!entity_manager.entity_list[i]._inuse)
        continue;

        if(entity_manager.entity_list[i].energyMax > 0)
        {
            return &entity_manager.entity_list[i];
        }
    }

    return NULL;
}