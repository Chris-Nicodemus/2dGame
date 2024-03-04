#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_input.h"
#include "player.h"
#include "card.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);
void player_leftClick(Entity *self);
void player_rightClick(Entity *self);
void player_new_deck(Entity *self);


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
    ent->scale = vector2d(2.5,2.5);
    ent->drawOffsetX = -128 * ent->scale.y * 0.5;
    ent->drawOffsetY = -128 * ent->scale.y * 0.5;
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * 128,ent->scale.y * 128);
    ent->frame = 96;
    ent->flip = vector2d(1,0);
    ent->think = player_think;
    ent->update = player_update;
    ent->free = player_free;
    ent->leftClick = player_leftClick;
    ent->rightClick = player_rightClick;
    ent->healthMax = 89;
    ent->health = ent->healthMax; 
    ent->energyMax = 3;
    ent->energy = ent->energyMax;
    ent->type = Player;

    ent->data = gfc_list_new();
    
    //deck
    gfc_list_append(ent->data,gfc_list_new());
    //currentdeck
    gfc_list_append(ent->data,gfc_list_new());
    //discard pile
    gfc_list_append(ent->data,gfc_list_new());
    //hand
    gfc_list_append(ent->data,gfc_list_new());

    //player_new_deck(ent);
    

    //copies decklist into current deck
    //gfc_list_append(ent->data,gfc_list_copy(gfc_list_get_nth(ent->data,0)));

    //discard pile
    //gfc_list_append(ent->data,gfc_list_new());
    //gfc_list_append(gfc_list_get_nth(ent->data,2),"test");

    //hand
    //gfc_list_append(ent->data,gfc_list_new());
    //player_shuffle(ent);
    //player_draw(ent,10);

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
    //slog("frame: %f", self->frame);
    self->drawOffsetX = -128 * self->scale.y * 0.5;
    self->drawOffsetY = -128 * self->scale.y * 0.5;
    self->bounds = gfc_rect(self->position.x + self->drawOffsetX,self->position.y + self->drawOffsetY,self->scale.x * 128,self->scale.y * 128);
    //slog("frame: %i",(int)self->frame);
}

void player_free(Entity *self)
{
    if(!self)
    return;
    int i;


    while(gfc_list_get_count(self->data) > 0)
    {
        i = gfc_list_get_count(self->data) - 1;
        gfc_list_delete(gfc_list_get_nth(self->data,i));
        gfc_list_delete_last(self->data);
    }

    gfc_list_delete(self->data);
    self->data = NULL;
}

void player_leftClick(Entity *self)
{
    if(!self) return;

    slog("player was left clicked");

    /*int i;

    slog("hand");
    for(i = 0; i < gfc_list_get_count(gfc_list_get_nth(self->data,3)); i++)
    {
        slog("%s",gfc_list_get_nth(gfc_list_get_nth(self->data,3),i));
    }

    slog("deck");
    for(i = 0; i < gfc_list_get_count(gfc_list_get_nth(self->data,1)); i++)
    {
        slog("%s",gfc_list_get_nth(gfc_list_get_nth(self->data,1),i));
    }*/
}

void player_rightClick(Entity *self)
{
    if(!self) return;

    slog("player was right clicked");

    /*int i;

    for(i = 0; i < gfc_list_get_count(gfc_list_get_nth(self->data,1)); i++)
    {
        slog("%s",gfc_list_get_nth(gfc_list_get_nth(self->data,1),i));
    }*/

    //slog("discard %s",gfc_list_get_nth(gfc_list_get_nth(self->data,2),0));
}

void player_new_deck(Entity *self)
{
    int i;

    if(!self) return;

    List *deck = gfc_list_get_nth(self->data,0);

    for(i = 0; i < 5; i++)
    {
        gfc_list_append(deck,"strike");
        gfc_list_append(deck,"defend");
    }

    gfc_list_concat(gfc_list_get_nth(self->data,1),deck);
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

void player_draw(Entity *self, Uint8 num)
{
    if(!self) return;

    //always draw at least one card
    if(!num) num = 1;

    List *hand = gfc_list_get_nth(self->data,3);
    List *currentDeck = gfc_list_get_nth(self->data,1);
    int cards = gfc_list_get_count(hand);
    Entity *card;    
    char *name;

    while((cards + 1  <= 10) && num > 0)
    {
        //shuffle discard pile into deck if deck empty
        if(gfc_list_get_count(currentDeck) == 0)
        {
            player_shuffle(self);
        }    
        //slog("getting here");
        name = gfc_list_get_nth(currentDeck,0); //gets top card of deck
        //slog("%s",name);
        card = card_new(name, self);
        gfc_list_append(hand,card); //add card to hand
        gfc_list_delete_nth(currentDeck,0); //removes card from top of deck

        num--;
        cards++;
    }

    player_arrange_hand(self);
}

void player_arrange_hand(Entity *self)
{
    if(!self) return;
    
    int i;
    List *hand = gfc_list_get_nth(self->data,3);

    Entity *card = gfc_list_get_nth(hand,0);
    card->position = vector2d(71, 1154);

    for(i = 0; i < gfc_list_get_count(hand); i++)
    {
        card = gfc_list_get_nth(hand,i);

        card->position = vector2d(240 * i + 20, 1154);
    }
}

void player_save_deck(Entity *self)
{
    if(!self) return;

    List *d = gfc_list_get_nth(self->data,0);
    char *entry = gfc_list_get_nth(d,0);
    FILE *deck;
    int i;
    int count = gfc_list_get_count(d);

    deck = fopen("config/deck.json","w");

    fprintf(deck,"{\n\t\"deck\":\n\t[\n\t\t\"%s\"",entry);

    for(i = 1; i < count; i++)
    {
        entry = gfc_list_get_nth(d,i);
        fprintf(deck,",\n\t\t\"%s\"",entry);
    }
    
    fprintf(deck,"\n\t]\n}");

    fclose(deck);
}

void player_load_deck(Entity *self)
{
    SJson *json,*dson;
    List *deck = gfc_list_get_nth(self->data,0);
    List *currentDeck = gfc_list_get_nth(self->data,1);
    int i;
    char *name;
    if(!self)
    return;

    json = sj_load("config/deck.json");
    if(!json)
    {
        slog("failed to l0ad json");
        return;
    }

    dson = sj_object_get_value(json,"deck");
    if(!dson)
    {
        slog("failed to get deck");
        return;
    }

    for(i = 0; i < sj_array_get_count(dson);i++)
    {
        //slog("%s",sj_get_string_value(sj_array_get_nth(dson,i)));
        
        name = sj_get_string_value(sj_array_get_nth(dson,i));
        gfc_list_append(deck,name);
    }
    gfc_list_concat(currentDeck,deck);
}
//spritesheet goes from 0 to 147
//96 to 111 is walking loop