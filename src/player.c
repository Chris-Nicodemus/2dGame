#include "simple_logger.h"
#include "gfc_input.h"
#include "player.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);
void player_leftClick(Entity *self);
void player_rightClick(Entity *self);
void new_deck(Entity *self);


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
    ent->drawOffset = -64;
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffset,ent->position.y + ent->drawOffset,ent->scale.x * 128,ent->scale.y * 128);
    ent->frame = 96;
    ent->flip = vector2d(1,0);
    ent->think = player_think;
    ent->update = player_update;
    ent->free = player_free;
    ent->leftClick = player_leftClick;
    ent->rightClick = player_rightClick;

    ent->data = gfc_list_new();
    
    new_deck(ent);

    //copies decklist into current deck
    gfc_list_append(ent->data,gfc_list_copy(gfc_list_get_nth(ent->data,0)));

    //discard pile
    gfc_list_append(ent->data,gfc_list_new());
    gfc_list_append(gfc_list_get_nth(ent->data,2),"test");

    player_shuffle(ent);

    gfc_list_append(gfc_list_get_nth(ent->data,2),"test");
    //gfc_list_append(gfc_list_get_nth(ent->data,2),"test");
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
    
    self->bounds = gfc_rect(self->position.x + self->drawOffset,self->position.y + self->drawOffset,self->scale.x * 128,self->scale.y * 128);
    //slog("frame: %i",(int)self->frame);
}

void player_free(Entity *self)
{
    if(!self)
    return;

    self->data = NULL;
}

void player_leftClick(Entity *self)
{
    if(!self) return;

    slog("player was left clicked");

    int i;

    /*for(i = 0; i < gfc_list_get_count(gfc_list_get_nth(self->data,0)); i++)
    {
        slog("%s",gfc_list_get_nth(gfc_list_get_nth(self->data,0),i));
    }*/
}

void player_rightClick(Entity *self)
{
    if(!self) return;

    slog("player was right clicked");

    int i;

    /*for(i = 0; i < gfc_list_get_count(gfc_list_get_nth(self->data,1)); i++)
    {
        slog("%s",gfc_list_get_nth(gfc_list_get_nth(self->data,1),i));
    }*/

    //slog("discard %s",gfc_list_get_nth(gfc_list_get_nth(self->data,2),0));
}

void new_deck(Entity *self)
{
    int i = 0;

    if(!self) return;

    List *deck = gfc_list_new();

    for(i = 0; i < 5; i++)
    {
        gfc_list_append(deck,"strike");
        gfc_list_append(deck,"defend");
    }

    gfc_list_append(self->data, deck);
}

void player_shuffle(Entity *self)
{
    if(!self) return;

    List *currentDeck = gfc_list_get_nth(self->data,1);
    List *discard = gfc_list_get_nth(self->data,2);
    int count;
    int i,r;

    //if there is a discard pile
    if(gfc_list_get_count(discard) > 0)
    {
        gfc_list_concat(currentDeck,discard);

        //empty discard
        while(gfc_list_get_count(discard) > 0)
        {
            gfc_list_delete_last(discard);
        }
        //gfc_list_insert(self->data,gfc_list_new(),2);
    }

    count = gfc_list_get_count(currentDeck);

    for(i = 0; i < count; i++)
    {
        r = (count - 1) * gfc_random();
        gfc_list_swap_indices(currentDeck,i,r);
    }
}
//spritesheet goes from 0 to 147
//96 to 111 is walking loop