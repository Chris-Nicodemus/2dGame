#include "simple_logger.h"
#include "gfc_input.h"
#include "card.h"
#include "icon.h"

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
    slog("card was left clicked");

    if(strcmp(self->data,"strike") == 0)
    {
        //slog("getting here");
        if(self->owner->energy < 1)
        return;

        target = true;
        targetsNeeded = 1;
    }

    usedCard = self;
}

void card_rightClick(Entity *self)
{
    if(!self) return;

    slog("card was right clicked");
}

char *card_get_random()
{
    int i;
    i = (int) (gfc_random() * 2);

    switch(i)
    {
        case 0:
        return "strike";

        case 1:
        return "defend";

        default:
        return "fail";
    }

    return "fail";
}

char *card_toString(Entity *self)
{
    if(!self) return;

    if(!strcmp(self->data,"strike")) return "strike";

    if(!strcmp(self->data, "defend")) return "defend";

    return "fail";
}
//51 pixel space between
//1154 pixels down
//189 width
//245.7 height