#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_audio.h"
#include "card.h"
#include "icon.h"
#include "font.h"
#include "player.h"
#include "enemy.h"
#include "consumable.h"

void card_think(Entity *self);
void card_update(Entity *self);
void card_free(Entity *self);
void card_leftClick(Entity *self);
void card_rightClick(Entity *self);
void card_draw(Entity *self);

void find_sprite(Entity *self);

extern State state;
extern EventType event;
extern Bool turn;
extern Bool target;
extern List *targets;
extern int targetsNeeded;
extern Entity *usedCard;
extern Gold gold;

Sound *flip;
Sound *charge;
extern Sound *gunshot;
Sound *fly;

Entity *card_new(char *name, Entity *player)
{
    if(!flip)
    flip = gfc_sound_load("audio/flipcard.mp3",0.5,0);

    if(!charge)
    charge = gfc_sound_load("audio/charge-up.wav",0.5,1);

    if(!fly)
    fly = gfc_sound_load("audio/fly.mp3",1,0);

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
    else if(!strcmp(self->data,"defensiveAction"))
    {
        self->sprite = gf2d_sprite_load_image("images/cards/defensiveAction.png");
    }
    else if(!strcmp(self->data, "hunker"))
    {
        self->sprite = gf2d_sprite_load_image("images/cards/hunker.png");
    }
    else if(!strcmp(self->data,"skies"))
    {
        self->sprite = gf2d_sprite_load_image("images/cards/skies.png");
    }
    else if(!strcmp(self->data,"slipstream"))
    {
        self->sprite = gf2d_sprite_load_image("images/cards/slipstream.png");
    }
    else if(!strcmp(self->data, "airstrike"))
    {
        self->sprite = gf2d_sprite_load_image("images/cards/airstrike.png");
    }
    else if(!strcmp(self->data, "carpet"))
    {
        self->sprite  = gf2d_sprite_load_image("images/cards/carpet.png");
    }
    else if(!strcmp(self->data, "unleash"))
    {
        self->sprite  = gf2d_sprite_load_image("images/cards/unleash.png");
    }
    else if(!strcmp(self->data, "skyfall"))
    {
        self->sprite  = gf2d_sprite_load_image("images/cards/skyfall.png");
    }
}

void card_add_to_deck(Entity *self)
{
    if(!self) return;

    Entity *player = entity_get_player();

    if(!player) return;

    List *deck = gfc_list_get_nth(player->data, 0);

    slog("Added %s to deck", self->data);
    gfc_list_append(deck, self->data);

    entity_free(self);

    if(state == Event && event != Shop)
    {
        event_close();
    }
}

void card_think(Entity *self)
{
    if(!self)
    return;

    if(self->owner)
    {
        if(self->mouse && !self->oldMouse)
        {
            gfc_sound_play(flip,0,0.5,gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) % 8, 0);  
            //slog("playing sound"); 
        }

        if(!strcmp(self->data,"carpet") && self->owner->airborne)
        {
            self->sprite = gf2d_sprite_load_image("images/cards/carpetUp.png");
            self->data = "carpetUp";
        }

        if(!strcmp(self->data,"carpetUp") && !self->owner->airborne)
        {
            self->sprite = gf2d_sprite_load_image("images/cards/carpet.png");
            self->data = "carpet";
        }
    }

    if(self->mouse)
    {
        //things to do when being moused on
    }
}

void card_update(Entity *self)
{
    if(!self)
    return;

}

void card_free(Entity *self)
{
    if(!self)
    return;

}

void card_leftClick(Entity *self)
{
    if(!self) return;

    //add earned card to deck
    if(self->gift)
    {
        card_add_to_deck(self);
    }

    //all the resons to not do anything
    
    if(event == Shop)
    {
        slog("Shop card click");
        if(!self->shop) return;

        if(entity_get_player()->gold >= self->gold)
        {
            entity_get_player()->gold = entity_get_player()->gold - self->gold; 
            card_add_to_deck(self);
        }
        else
        {
            text_new("Not Enough Gold",FS_Large,vector2d(150,325),vector2d(3,3),GFC_COLOR_WHITE, 1000, TT_Event);
        }
    }
    if(!self->owner) return;

    if(state != Combat && state != Multiplayer && event != ChestFight) return;

    //slog("click");

    if(state == Combat || event == ChestFight)
    {
        if(!turn) return;

        if(target) return;
    }
    //slog("card was left clicked");

    if(strcmp(self->data,"strike") == 0)
    {
        //slog("getting here");
        if(self->owner->energy < 1)
        return;

        if(state == Combat || event == ChestFight)
        {
            target = true;
            targetsNeeded = 1;
            usedCard = self;
        }
        else
        {
            gfc_sound_play(gunshot,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);
            if(self->owner->type == Player)
            {
                player_damage(entity_get_player2(),self->owner, 6, Basic);
                player_discard(self->owner,self);
            }
            else
            {
                player_damage(entity_get_player(),self->owner, 6, Basic);
                player_discard(self->owner,self);
            }
            self->owner->energy = self->owner->energy - 1;
              
        }
    }
    else if(!strcmp("defend",self->data))
    {
        if(self->owner->energy < 1) return;

        self->owner->energy = self->owner->energy - 1;
        self->owner->block = self->owner->block + 5;
        gfc_sound_play(charge,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  

        player_discard(self->owner,self);
    }
    else if(!strcmp("defensiveAction",self->data))
    {
        if(self->owner->energy < 1) return;

        self->owner->energy = self->owner->energy - 1;
        self->owner->block = self->owner->block + 5;
        self->owner->airborne = true;
        gfc_sound_play(charge,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  

        player_discard(self->owner,self);
    }
    else if(!strcmp("hunker", self->data))
    {
        if(self->owner->energy < 2) return;

        self->owner->energy = self->owner->energy - 2;
        gfc_sound_play(charge,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  

        if(self->owner->airborne)
        {
            self->owner->airborne = false;
            self->owner->block = self->owner->block + 16;
        }
        else
        {
            self->owner->block = self->owner->block + 8;
        }

        player_discard(self->owner,self);
    }
    else if(!strcmp("skies",self->data))
    {
        if(self->owner->energy < 1) return;


        if(state == Combat || event == ChestFight)
        {
            target = true;
            targetsNeeded = 1;
            usedCard = self;
        }
        else
        {
            gfc_sound_play(gunshot,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  
            if(self->owner->type == Player)
            {
                player_damage(entity_get_player2(),self->owner, 3, Basic);
                player_discard(self->owner,self);
            }
            else
            {
                player_damage(entity_get_player(),self->owner, 3, Basic);
                player_discard(self->owner,self);
            }
            self->owner->airborne = true;
            self->owner->energy = self->owner->energy - 1;
        }
    }
    else if(!strcmp("slipstream",self->data))
    {
        if(self->owner->energy < 1) return;

        if(self->owner->airborne)
        {
            player_draw(self->owner,2);
        }        
        else
        {
            player_draw(self->owner,1);
        }

        gfc_sound_play(fly,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  
        self->owner->energy = self->owner->energy - 1;
        player_discard(self->owner,self);        
    }
    else if(!strcmp("airstrike",self->data))
    {
        //multiplayer implementation still needed from here down
        if(self->owner->energy < 2) return;

        if(state == Combat || event == ChestFight)
        {
            target = true;
            targetsNeeded = 1;
            usedCard = self;
        }
        else
        {
            int damage;

            if(self->owner->airborne)
                damage = 14;
            else
                damage = 8;

            gfc_sound_play(gunshot,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  

            if(self->owner->type == Player)
            {
                player_damage(entity_get_player2(),self->owner, damage, Basic);
                player_discard(self->owner,self);
            }
            else
            {
                player_damage(entity_get_player(),self->owner, damage, Basic);
                player_discard(self->owner,self);
            }

            self->owner->energy = self->owner->energy - 2;
        }
    }
    else if(!strcmp("carpet",self->data))
    {
        if(self->owner->energy < 3) return;

        if(state == Combat || event == ChestFight)
            enemy_damage_all(self->owner,6,0);
        else
        {
            if(self->owner->type == Player)
                player_damage(entity_get_player2(),self->owner, 6, Basic);
            else
                player_damage(entity_get_player(),self->owner, 6, Basic);
        }
        self->owner->energy = self->owner->energy - 3;
        player_discard(self->owner,self);    
    }
    else if(!strcmp("carpetUp",self->data))
    {
        if(self->owner->energy < 1) return;

         if(state == Combat || event == ChestFight)
            enemy_damage_all(self->owner,6,0);
        else
        {
            if(self->owner->type == Player)
                player_damage(entity_get_player2(),self->owner, 6, Basic);
            else
                player_damage(entity_get_player(),self->owner, 6, Basic);
        }
        self->owner->energy = self->owner->energy - 1;
        gfc_sound_play(gunshot,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);   

        player_discard(self->owner,self);  
         
    }
    else if(!strcmp("unleash",self->data))
    {
        self->owner->energy = self->owner->energy + 2;
        //remove card from list
        List *hand = gfc_list_get_nth(self->owner->data, 3);
        gfc_list_delete_nth(hand, gfc_list_get_item_index(hand,self));

        gfc_sound_play(charge,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  
    
        player_arrange_hand(self->owner);
        entity_free(self);
    }
    else if(!strcmp("skyfall",self->data))
    {
        if(self->owner->energy < 3) return;

        if(state == Combat || event == ChestFight)
        {
            target = true;
            targetsNeeded = 1;
            usedCard = self;
        }
        else
        {
            if(self->owner->type == Player)
                player_damage(entity_get_player2(), self->owner, 20, Basic);
            else
                player_damage(entity_get_player(), self->owner, 20, Basic);
            self->owner->energy = self->owner->energy - 3;

            if(self->owner->health - 5 <= 0)
            self->owner->health = 0;
            else
            self->owner->health = self->owner->health - 5;

            if(self->owner->airborne)
            {
                self->owner->airborne = false;
                self->owner->energy = self->owner->energy + 2;
            }

            gfc_sound_play(gunshot,0,1,(gfc_list_get_item_index(gfc_list_get_nth(self->owner->data,3),self) + 1) % 8, 0);  

            if(self)
                player_discard(self->owner,self);  

            
        }
    }
}

void card_rightClick(Entity *self)
{
    if(!self) return;

    slog("card was right clicked");
}

char *card_get_random()
{
    int i;
    i = (int) (gfc_random() * 10);

    switch(i)
    {
        case 0:
        return "strike";

        case 1:
        return "defend";

        case 2:
        return "defensiveAction";

        case 3:
        return "hunker";

        case 4:
        return "skies";

        case 5:
        return "slipstream";

        case 6:
        return "airstrike";

        case 7:
        return "carpet";

        case 8:
        return "unleash";

        case 9:
        return "skyfall";

        default:
        return "fail";
    }

    return "fail";
}

char *card_toString(Entity *self)
{
    if(!self) return "fail";

    if(!strcmp(self->data,"strike")) return "strike";

    if(!strcmp(self->data, "defend")) return "defend";

    if(!strcmp(self->data, "defensiveAction")) return "defensiveAction";

    if(!strcmp(self->data, "hunker")) return "hunker";

    if(!strcmp(self->data, "skies")) return "skies";

    if(!strcmp(self->data, "slipstream")) return "slipstream";

    if(!strcmp(self->data, "airstrike")) return "airstrike";

    if(!strcmp(self->data, "carpet")) return "carpet";

    if(!strcmp(self->data, "carpetUp")) return "carpet";

    if(!strcmp(self->data, "unleash")) return "unleash";

    if(!strcmp(self->data, "skyfall")) return "skyfall";

    return "fail";
}

int get_gold_cost(Entity *self)
{
    if(!self) return 0;

    if(!strcmp(self->data,"strike")) return 10;

    if(!strcmp(self->data, "defend")) return 10;

    if(!strcmp(self->data, "defensiveAction")) return 25;

    if(!strcmp(self->data, "hunker")) return 30;

    if(!strcmp(self->data, "skies")) return 25;

    if(!strcmp(self->data, "slipstream")) return 40;

    if(!strcmp(self->data, "airstrike")) return 30;

    if(!strcmp(self->data, "carpet")) return 25;

    if(!strcmp(self->data, "unleash")) return 60;

    if(!strcmp(self->data, "skyfall")) return 50;

    return 0;
}

Entity *card_shop_new()
{
    Entity *ent = card_new(card_get_random(),NULL);

    if(!ent) return NULL;

    ent->shop = true;
    ent->draw = card_draw;

    ent->gold = get_gold_cost(ent);

    return ent;
}

void card_draw(Entity *self)
{
    char *text;
    Color color;
    if(!self) return;

    consumable_draw_gold(self);

    text = (char *)malloc(20);
    sprintf(text,"%i", self->gold);

    Vector2D pos = self->position;
    pos.x = pos.x + (gold.scale.x * 32) + 52.25;
    pos.y = pos.y + (self->scale.y * self->pixel.y) + 10;

    if(self->mouse)
        color = GFC_COLOR_WHITE;
    else
        color = gfc_color8(175, 99, 188, 255); //purple
    font_draw_text(text,FS_Medium,color, pos, vector2d(2,2));
}
//51 pixel space between
//1154 pixels down
//189 width
//245.7 height