#include "simple_logger.h"
#include "gfc_input.h"
#include "player.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);
void player_leftClick(Entity *self);
void player_rightClick(Entity *self);

Entity *player_new(Vector2D pos)
{
    Entity *ent = entity_new();

    if(!ent)
    {
        slog("Failed to spawn player");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/ed210.png",128,128,16,0);
    
    ent->position = pos;
    ent->scale = vector2d(1,1);
    ent->bounds = gfc_rect(ent->position.x,ent->position.y,ent->scale.x * 128,ent->scale.y * 128);
    ent->frame = 96;
    ent->flip = vector2d(1,0);
    ent->think = player_think;
    ent->update = player_update;
    ent->free = player_free;
    ent->leftClick = player_leftClick;
    ent->rightClick = player_rightClick;
    return ent;
}

void player_think(Entity *self)
{
    if(!self)
    return;


    if(self->mouse)
    {
        //things to do when being moused on
    }

    if(gfc_input_command_pressed("file"))
    {
        slog("input works");
    }
}

void player_update(Entity *self)
{
    if(!self)
    return;

    //slog("updating");
    self->frame = self->frame + 0.1;
    if(self->frame >= 111)
    {
        self->frame = 96;
    }
    
    self->bounds = gfc_rect(self->position.x,self->position.y,self->scale.x * 128,self->scale.y * 128);
    //slog("frame: %i",(int)self->frame);
}

void player_free(Entity *self)
{
    if(!self)
    return;
}

void player_leftClick(Entity *self)
{
    if(!self) return;

    slog("player was left clicked");
}

void player_rightClick(Entity *self)
{
    if(!self) return;
}
//spritesheet goes from 0 to 147
//96 to 111 is walking loop