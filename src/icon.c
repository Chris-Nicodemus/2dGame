#include "simple_logger.h"
#include "gfc_list.h"
#include "icon.h"

typedef struct
{
    Sprite *sprite;
    int x;  //pixel width
    int y;  //pixel height
    float scaleX;
    float scaleY;
    Bool noHighlight;
    Bool offset;
}IconInfo;

typedef struct
{
    IconInfo *icons;
    Uint32 iconMax;
}IconManager;

IconManager icon_manager = {0};

extern State state;

void icon_think(Entity *self);
void icon_update(Entity *self);
void icon_leftClick(Entity *self);
//void icon_rightClick(Entity *self);

void icon_quit()
{
    if(icon_manager.icons)
    free(icon_manager.icons);

    
    memset(&icon_manager,0,sizeof(IconManager));

    slog("icon close");
}

void icon_init()
{
    icon_manager.iconMax = 32;
    icon_manager.icons = gfc_allocate_array(sizeof(IconInfo),icon_manager.iconMax);

    IconInfo icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/questionMark.png");
    icon.x = 634;
    icon.y = 980;
    //no highlight and draw offset is on for all map sprites
    icon.noHighlight = true;
    icon.offset = true;
    icon.scaleX = 1;
    icon.scaleY = 1;

    icon_manager.icons[0] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/mapBattle.png");
    icon.x = 32;
    icon.y = 32;

    icon_manager.icons[1] = icon;
    
    //same size as before
    icon.sprite = gf2d_sprite_load_image("images/icons/mapExplore.png");
    
    icon_manager.icons[2] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/mapShop.png");

    icon_manager.icons[3] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/mapShrine.png");

    icon_manager.icons[4] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/battle.png");
    icon.x = 600;
    icon.y = 1240;
    icon.noHighlight = false;
    icon.offset = false;
    icon.scaleX = 1;
    icon.scaleY = 1;

    icon_manager.icons[5] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/explore.png");

    icon_manager.icons[6] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/shop.png");

    icon_manager.icons[7] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/shrine.png");

    icon_manager.icons[8] = icon;
    atexit(icon_quit);
}

void icon_set_sprite(Entity *self, Icons icon)
{
    IconInfo ref;
    if(!self) return;

    ref = icon_manager.icons[icon];
    self->sprite = ref.sprite;

    //slog("x: %f, y: %f", ref.scaleX,ref.scaleY);
    
    self->scale.x = ref.scaleX;
    self->scale.y = ref.scaleY;

    self->pixel = vector2d(ref.x,ref.y);

    if(ref.offset)
    {
        self->drawOffsetX = -ref.x * self->scale.x * .5;
        self->drawOffsetY = -ref.y * self->scale.y * .5;
    }
    else
    {
        self->drawOffsetX = 0;
        self->drawOffsetY = 0;
    }
    
    self->noHighlight = ref.noHighlight;

}

Entity *icon_new(Vector2D pos, Icons icon)
{
    Entity *ent = entity_new();

    ent->position = pos;
    icon_set_sprite(ent, icon);
    //ent->scale = vector2d(1,1);
    ent->think = icon_think;
    ent->update = icon_update;
    ent->leftClick = icon_leftClick;
    ent->type = Icon;
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * ent->pixel.x,ent->scale.y * ent->pixel.y);
    return ent;
}

void icon_think(Entity *self)
{
    if(!self) return;

    //IconInfo info = icon_manager.icons[(Icons)self->data];

    self->bounds = gfc_rect(self->position.x + self->drawOffsetX,self->position.y + self->drawOffsetY,self->scale.x * self->pixel.x,self->scale.y * self->pixel.y);
    //slog("%i",(int)self->data);
}

void icon_update(Entity *self)
{
    if(!self) return;

}

void icon_leftClick(Entity *self)
{
    if(!self) return;
}