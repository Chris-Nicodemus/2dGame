#include "simple_logger.h"
#include "gfc_input.h"
#include "card.h"
#include "icon.h"
#include "player.h"
#include "enemy.h"

void card_think(Entity *self);
void card_update(Entity *self);
void card_free(Entity *self);
void card_leftClick(Entity *self);
void card_rightClick(Entity *self);

void find_sprite(Entity *self);

extern State state;
extern Bool turn;
extern Bool target;
extern List *targets;
extern int targetsNeeded;
extern Entity *usedCard;

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

    if(state == Event)
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
    if(!self->owner) return;

    if(state != Combat) return;

    //slog("click");

    if(!turn) return;

    if(target) return;
    //slog("card was left clicked");

    if(strcmp(self->data,"strike") == 0)
    {
        //slog("getting here");
        if(self->owner->energy < 1)
        return;

        target = true;
        targetsNeeded = 1;
        usedCard = self;
    }
    else if(!strcmp("defend",self->data))
    {
        if(self->owner->energy < 1) return;

        self->owner->energy = self->owner->energy - 1;
        self->owner->block = self->owner->block + 5;

        player_discard(self->owner,self);
    }
    else if(!strcmp("defensiveAction",self->data))
    {
        if(self->owner->energy < 1) return;

        self->owner->energy = self->owner->energy - 1;
        self->owner->block = self->owner->block + 5;
        self->owner->airborne = true;

        player_discard(self->owner,self);
    }
    else if(!strcmp("hunker", self->data))
    {
        if(self->owner->energy < 2) return;

        self->owner->energy = self->owner->energy - 2;

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

        target = true;
        targetsNeeded = 1;
        usedCard = self;
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

        self->owner->energy = self->owner->energy - 1;
        player_discard(self->owner,self);        
    }
    else if(!strcmp("airstrike",self->data))
    {
        if(self->owner->energy < 2) return;

        target = true;
        targetsNeeded = 1;
        usedCard = self;
    }
    else if(!strcmp("carpet",self->data))
    {
        if(self->owner->energy < 3) return;

        enemy_damage_all(self->owner,6,0);
        self->owner->energy = self->owner->energy - 3;
        player_discard(self->owner,self);    
    }
    else if(!strcmp("carpetUp",self->data))
    {
        if(self->owner->energy < 1) return;

        enemy_damage_all(self->owner,6,0);
        self->owner->energy = self->owner->energy - 1;
        player_discard(self->owner,self);    
    }
    else if(!strcmp("unleash",self->data))
    {
        self->owner->energy = self->owner->energy + 2;
        //remove card from list
        List *hand = gfc_list_get_nth(self->owner->data, 3);
        gfc_list_delete_nth(hand, gfc_list_get_item_index(hand,self));
    
        player_arrange_hand(self->owner);
        entity_free(self);
    }
    else if(!strcmp("skyfall",self->data))
    {
        if(self->owner->energy < 3) return;

        target = true;
        targetsNeeded = 1;
        usedCard = self;
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
//51 pixel space between
//1154 pixels down
//189 width
//245.7 height