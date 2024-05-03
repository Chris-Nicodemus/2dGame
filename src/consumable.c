#include "simple_logger.h"
#include "gfc_list.h"
#include "consumable.h"
#include "font.h"
#include "player.h"

//struct for drawing gold more efficiently
typedef struct
{
    Sprite *sprite;
    Vector2D offset;
    Vector2D scale;
} Gold;

static Gold gold;
List *consumableList;

extern int maxConsumables;
extern State state;
extern EventType event;
extern Bool target;
extern Uint8 targetsNeeded;
extern Entity *usedCard;

int healthPotionStrength = 10;
int shieldPotionStrength = 15;

void consumable_close()
{
    if(consumableList)
    {
        while(gfc_list_get_count(consumableList) > 0)
        {
            gfc_list_delete_last(consumableList);
        }

        gfc_list_delete(consumableList);
    }
}

void consumable_init()
{
    if(!consumableList)
        consumableList = gfc_list_new();
    else
    {
        slog("consumables already started");
        return;
    }
    
        gold.sprite = gf2d_sprite_load_image("images/icons/gold.png");
        gold.offset = vector2d(0,0);
        gold.scale = vector2d(1,1);

        atexit(consumable_close);
}

void consumable_leftClick(Entity *self);
void consumable_rightClick(Entity *self);
void consumable_think(Entity *self);
void consumable_update(Entity *self);
void consumable_free(Entity *self);
void consumable_draw(Entity *self);

void consumable_set(Entity *self, char *name)
{
    if(!self) return;

    if(!strcmp(name,"fire"))
    {
        self->sprite = gf2d_sprite_load_image("images/icons/firePotion.png");
        self->data = "fire";

        if(self->shop)
        self->gold = 20;
        return;
    }

    if(!strcmp(name,"health"))
    {
        self->sprite = gf2d_sprite_load_image("images/icons/healthPotion.png");
        self->data = "health";
        return;
    }

    if(!strcmp(name,"shield"))
    {
        self->sprite = gf2d_sprite_load_image("images/icons/shieldPotion.png");
        self->data = "shield";
        return;
    }

    if(!strcmp(name,"energy"))
    {
        self->sprite = gf2d_sprite_load_image("images/icons/energyPotion.png");
        self->data = "energy";
        return;
    }

    if(!strcmp(name,"sky"))
    {
        self->sprite = gf2d_sprite_load_image("images/icons/skyPotion.png");
        self->data = "sky";
        return;
    }
}

void consumable_free(Entity *self)
{
    List *consumables;
    if(!self) return;

    if(self->owner)
    {
        consumables = gfc_list_get_nth(self->owner->data, 4);
        gfc_list_delete_data(consumables,self);
        player_arrange_consumables(self->owner);
    }

    gfc_list_delete_nth(consumableList,gfc_list_get_item_index(consumableList,self));
    
    self->data = NULL;
}

Entity *consumable_new(Vector2D pos, char *name, Bool owned)
{
    List *playerConsumables;
    Entity *ent = entity_new();

    if(!ent)
    return NULL;

    ent->position = pos;
    ent->data = name;
    ent->pixel = vector2d(32,32);
    ent->scale = vector2d(3,3);
    ent->type = Consumable;

    if(owned)
    {
        playerConsumables = gfc_list_get_nth(entity_get_player()->data, 4);
        if((gfc_list_get_count(playerConsumables) + 1) > maxConsumables)
        {
            slog("Too many consumables!");
            return NULL;
        }
        ent->owner = entity_get_player();
        gfc_list_append(playerConsumables,ent);
        player_arrange_consumables(ent->owner);
    }
    else
    {
        ent->shop = true;
        ent->draw = consumable_draw;
    }

    ent->bounds = gfc_rect(ent->position.x,ent->position.y, ent->pixel.x * ent->scale.x, ent->pixel.y * ent->scale.y);
    consumable_set(ent,name);

    if(!ent->sprite) return NULL;

    ent->leftClick = consumable_leftClick;
    ent->rightClick = consumable_rightClick;
    ent->free = consumable_free;
    gfc_list_append(consumableList, ent);
    return ent;
}

void consumable_use(Entity *self)
{
    if(!self) return;

    if(!strcmp(self->data,"fire"))
    {
        target = true;
        targetsNeeded = 1;
        usedCard = self;
    }
    else if(!strcmp(self->data, "health"))
    {
        if(self->owner->health == self->owner->healthMax) return;

        if((self->owner->health + healthPotionStrength) > self->owner->healthMax)
            self->owner->health = self->owner->healthMax;
        else
            self->owner->health = self->owner->health + healthPotionStrength;

        entity_free(self);
        return;
    }
    else if(!strcmp(self->data, "shield"))
    {
        self->owner->block = self->owner->block + 15;
        entity_free(self);
        return;
    }
    else if(!strcmp(self->data, "energy"))
    {
        self->owner->energy = self->owner->energy + 2;
        entity_free(self);
        return;
    }
    else if(!strcmp(self->data, "sky"))
    {
        if(self->owner->airborne) return;

        self->owner->airborne = true;
        entity_free(self);
        return;
    }
}

void consumable_leftClick(Entity *self)
{
    if(!self) return;

    switch (state)
    {
    case Combat:
        if(target) return;
        
        if(self->owner)
        consumable_use(self);
        else
        slog("there is a non player owned consumable in the combat screen!");
        break;
    case Event:

        break;
    default:
        slog("Consumable clicked when not in combat or event");
        break;
    }
}

void consumable_rightClick(Entity *self)
{
    if(!self) return;

    if(self->owner)
    entity_free(self);
}

void consumable_draw_gold(Entity *self)
{
    if(!self) return;

    Vector2D pos = self->position;
    pos.x = pos.x + 10;
    pos.y = pos.y + (self->scale.y * self->pixel.y);
    
    if(gold.sprite)
    {
        gf2d_sprite_render(gold.sprite,pos,&gold.scale,NULL,NULL,&self->flip,NULL,NULL,0);
    }
}
void consumable_draw(Entity *self)
{
    char *text;
    Color color;
    if(!self) return;

    //draw gold
    text = (char *)malloc(20);
    sprintf(text,"%i", self->gold);
    consumable_draw_gold(self);

    Vector2D pos = self->position;
    pos.x = pos.x + (gold.scale.x * 32) + 15;
    pos.y = pos.y + (self->scale.y * self->pixel.y) + 10;

    if(self->mouse)
        color = GFC_COLOR_WHITE;
    else
        color = gfc_color8(175, 99, 188, 255); //purple
    font_draw_text(text,FS_Medium,color, pos, vector2d(2,2));
}