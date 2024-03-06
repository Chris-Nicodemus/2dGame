#include "simple_logger.h"
#include "gfc_input.h"
#include "card.h"

void card_think(Entity *self);
void card_update(Entity *self);
void card_free(Entity *self);
void card_leftClick(Entity *self);
void card_rightClick(Entity *self);

void find_sprite(Entity *self);

Entity *card_new(char *name, Entity *player)
{
    Entity *ent = entity_new();

    if(!ent)
    {
        slog("Failed to spawn card");
        return NULL;
    }

    ent->data = name;
    find_sprite(ent);
    
    ent->owner = player;
    ent->position = vector2d(0,0);
    ent->scale = vector2d(.18,.18);
    ent->pixel = vector2d(1050,1365);
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * ent->pixel.x,ent->scale.y * ent->pixel.y);
    ent->think = card_think;
    ent->update = card_update;
    ent->free = card_free;
    ent->leftClick = card_leftClick;
    ent->rightClick = card_rightClick;
    ent->type = Card;

    return ent;
}

void find_sprite(Entity *self)
{
    if(!self) return;
    //slog("%s",self->data);
    if(strcmp(self->data,"strike") == 0)
    {
        //slog("getting to strike");
        self->sprite = gf2d_sprite_load_image("images/cards/strike.png");    
    }
    else if(strcmp(self->data,"defend") == 0)
    {
        self->sprite = gf2d_sprite_load_image("images/cards/defend.png");
    }
}

void card_think(Entity *self)
{
    if(!self)
    return;


    if(self->mouse)
    {
        //things to do when being moused on
    }
}

void card_update(Entity *self)
{
    if(!self)
    return;

    self->bounds = gfc_rect(self->position.x + self->drawOffsetX,self->position.y + self->drawOffsetY,self->scale.x * self->pixel.x,self->scale.y * self->pixel.y);
}

void card_free(Entity *self)
{
    if(!self)
    return;

    self->data = NULL;
}

void card_leftClick(Entity *self)
{
    if(!self) return;

    slog("card was left clicked");
}

void card_rightClick(Entity *self)
{
    if(!self) return;

    slog("card was right clicked");
}

//51 pixel space between
//1154 pixels down
//189 width
//245.7 height