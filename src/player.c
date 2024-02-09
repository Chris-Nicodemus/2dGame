#include "simple_logger.h"
#include "player.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);

Entity *player_new()
{
    Entity *ent = entity_new();

    if(!ent)
    {
        slog("Failed to spawn player");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/ed210.png",128,128,16,0);

    ent->position = vector2d(0,0);
    ent->frame = 96;

    ent->think = player_think;
    ent->update = player_update;
    ent->free = player_free;
    return ent;
}

void player_think(Entity *self)
{
    if(!self)
    return;
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
    slog("frame: %i",(int)self->frame);
}

void player_free(Entity *self)
{
    if(!self)
    return;
}

//spritesheet goes from 0 to 147
//96 to 111 is walking loop