#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_input.h"
#include "gf2d_draw.h"
#include "font.h"
#include "player.h"
#include "card.h"
#include "enemy.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);
void player_leftClick(Entity *self);
void player_rightClick(Entity *self);
void player_new_deck(Entity *self);
void player_ui(Entity *self);

extern State state;
extern List *deckDisplay;
extern List *targets;
extern Bool turn;

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
    ent->pixel = vector2d(128,128);
    ent->drawOffsetX = -ent->pixel.x * ent->scale.x * 0.5;
    ent->drawOffsetY = -ent->pixel.y * ent->scale.y * 0.5;
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * ent->pixel.x,ent->scale.y * ent->pixel.y);
    ent->frameStart = 96;
    ent->frame = ent->frameStart;
    ent->frameEnd = 111;
    ent->flip = vector2d(1,0);
    ent->think = player_think;
    ent->update = player_update;
    ent->free = player_free;
    ent->leftClick = player_leftClick;
    ent->rightClick = player_rightClick;
    ent->draw = player_ui;
    ent->healthMax = 89;
    ent->health = 45;//ent->healthMax; 
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
    /*self->frame = self->frame + 0.1;
    if(self->frame >= self->frameEnd)
    {
        self->frame = self->frameStart;
    }*/
    //slog("frame: %f", self->frame);
    self->drawOffsetX = -self->pixel.x * self->scale.y * 0.5;
    self->drawOffsetY = -self->pixel.y * self->scale.y * 0.5;
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
        //slog("drawing");
        //shuffle discard pile into deck if deck empty
        if(gfc_list_get_count(currentDeck) == 0)
        {
            player_shuffle(self);

            if(gfc_list_get_count(currentDeck) == 0)
            {
                slog("deck empty!");
                break;
            }
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
    if(!card)
    return;

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

void player_ui(Entity *self)
{
    float healthRatio;
    Rect damage,health,energyDisplay;
    //Circle energyDisplay = gfc_circle(130,1000,100);
    Color purple = gfc_color8(175, 99, 188, 255);
    Color red = gfc_color8(100,0,0,255);
    Color green = gfc_color8(50,208,0,255);
    float barX,barY;
    float barLength, barWidth = 32;
    char *text = (char *)malloc(sizeof(char) * (10));
    
    if(!self) return;

    barLength = 128 * self->scale.x;
    healthRatio = (float)self->health / (float)self->healthMax;
            
    barX = self->position.x - (64 * self->scale.x);
    barY = self->position.y + (64 * self->scale.y);

    damage = gfc_rect(barX,barY, barLength,barWidth);
    health = gfc_rect(barX,barY,barLength * healthRatio, barWidth);
            
    gf2d_draw_rect_filled(damage,red);
    gf2d_draw_rect_filled(health,green);

    sprintf(text,"%i / %i",self->health,self->healthMax);

    font_draw_text(text,FS_Small, gfc_color(1,1,1,1), vector2d(self->position.x - (self->pixel.x * 0.5 * self->scale.x), self->position.y + (self->pixel.y * 0.5 * self->scale.y) + (barWidth *0.33)),vector2d(1.5,1.5));

    sprintf(text,"%i / %i",self->energy,self->energyMax);

    energyDisplay = gfc_rect(85,950,100,100);
    gf2d_draw_rect_filled(energyDisplay,purple);

    font_draw_text(text,FS_Small, gfc_color(1,1,1,1), vector2d(105, 1000) ,vector2d(1.5,1.5));
}

void player_deck_arrange(Entity *self)
{
    int i, j;
    int row = 0;
    int xOffset = 1;
    float vertSpace, horiSpace;

    Entity *card;

    if(!self) return;

    vertSpace = 49.5;
    horiSpace = 42.3;

    j = gfc_list_get_count(deckDisplay);
    for(i = 0; i < j; i++)
    {
        card = gfc_list_get_nth(deckDisplay,i);
        card->position = vector2d(380.0 + (horiSpace + (card->pixel.x * card->scale.x)) * xOffset, (vertSpace + card->pixel.y * card->scale.y) * row);
        //slog("x: %f, y: %f",card->position.x,card->position.y);
        //slog("%i", (i + 1) % 5);
        if((i + 1) % 5 == 0)
        {
            row++;
            xOffset = 0;
            //slog("i: %i, row: %i",i, row);
        }
        xOffset++;
    }

    //slog("%f, %f", card->position.x,card->position.y);
    //slog("%i", row);

}

void player_deck_check(Entity *self)
{
    Entity *card;
    List *deck;
    int i;

    if(!self) return;

    deck = gfc_list_get_nth(self->data,0);

    if(gfc_list_get_count(deckDisplay) != gfc_list_get_count(deck))
    {
        while(gfc_list_get_count(deckDisplay) > 0)
        {
            i = gfc_list_get_count(deckDisplay) - 1;
            entity_free(gfc_list_get_nth(deckDisplay,i));
            gfc_list_delete_last(deckDisplay);
        }

        for(i = 0; i < gfc_list_get_count(deck); i++)
        {
            card = card_new(gfc_list_get_nth(deck, i),NULL);
            gfc_list_append(deckDisplay,card);
        }

        player_deck_arrange(self);
    }
}

void player_show_deck(Entity *self)
{
    Rect screen;
    Color dark;
    if(!self) return;
    
    screen = gfc_rect(480,0,1440,1440);
    dark = gfc_color8(50,50,50,128);

    //this sets the deck to be drawn if it does not have the same amound of elements as the player deck
    player_deck_check(self);
    
    
    gf2d_draw_rect_filled(screen,dark);
}

void player_show_deck_close()
{
    int i;
    while(gfc_list_get_count(deckDisplay) > 0)
    {
        i = gfc_list_get_count(deckDisplay) - 1;
        entity_free(gfc_list_get_nth(deckDisplay,i));
        gfc_list_delete_last(deckDisplay);
    }
}

void player_discard(Entity *self, Entity *card)
{
    List *discard;
    List *hand;

    if(!self) return;

    if(!card) return;

    discard = gfc_list_get_nth(self->data,2);
    hand = gfc_list_get_nth(self->data,3);

    char* name = card_toString(card);
    
    gfc_list_append(discard,name);
    
    //delete card
    gfc_list_delete_nth(hand, gfc_list_get_item_index(hand,card));
    entity_free(card);

    player_arrange_hand(self);
}

void player_discard_hand(Entity *self)
{
    List *hand;
    Entity *card;
    int i;
    if(!self) return;

    hand = gfc_list_get_nth(self->data,3);
    if(!hand)
    {
        slog("failed to get hand");
        return;
    }

    while(gfc_list_get_count(hand) > 0)
    {
        i = gfc_list_get_count(hand) - 1;
        card = gfc_list_get_nth(hand,i);
        //slog("happening");
        player_discard(self,card);
    }

    //slog("%i",gfc_list_get_count(hand));
}

void player_play_card(Entity *self, Entity *card)
{
    Entity *target;
    Bool success;
    if(!self) return;

    if(!card) return;

    if(!strcmp(card->data,"strike"))
    {
        target = gfc_list_get_nth(targets,0);
        if(!target)
        {
            slog("invalid target");
            return;
        }
        success = true;
        enemy_damage(target,self,6,0);
        self->energy = self->energy - 1;
    }

    if(success)
    {
        //add card to discard
        player_discard(self,card);
    }
}

void player_end_turn(Entity *self)
{
    if(!self) return;

    //slog("happening");
    turn = false;
    player_discard_hand(self);
    player_draw(self,5);
    self->energy = self->energyMax;
    //slog("happening 2");
    turn = true;
}

void player_combat_start(Entity *self)
{
    int i;
    if(!self) return;

    if(state != Combat)
    state = Combat;

    List *deck = gfc_list_get_nth(self->data,0);
    List *current = gfc_list_get_nth(self->data,1);

    //current deck
    while(gfc_list_get_count(current) > 0)
    {
        i = gfc_list_get_count(current) - 1;

        //remove the string then delete
        /*if(gfc_list_get_nth(current,i))
        free(gfc_list_get_nth(current,i));*/

        gfc_list_delete_last(current);
    }

    //load current deck
    for(i = 0; i < gfc_list_get_count(deck); i++)
    {
        gfc_list_append(current,gfc_list_get_nth(deck,i));
    }
    
    player_shuffle(self);

    player_draw(self, 5);

    for(i = 0; i < gfc_list_get_count(current); i++)
    {
        //slog("%s",gfc_list_get_nth(current,i));
    }

    turn = true;
}
//spritesheet goes from 0 to 147
//96 to 111 is walking loop