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

static IconManager icon_manager = {0};
static List *tempIcons;
static World *world;
static Entity *player;

extern State state;
extern EventType event;
extern int level;

Uint32 graceInterval = 100;
Uint32 grace;

void icon_think(Entity *self);
void icon_update(Entity *self);
void icon_leftClick(Entity *self);
//void icon_rightClick(Entity *self);

void icon_quit()
{
    if(icon_manager.icons)
    free(icon_manager.icons);

    gfc_list_delete(tempIcons);

    memset(&icon_manager,0,sizeof(IconManager));

    slog("icon close");
}

void icon_init()
{
    icon_manager.iconMax = 32;
    icon_manager.icons = gfc_allocate_array(sizeof(IconInfo),icon_manager.iconMax);

    tempIcons = gfc_list_new();

    IconInfo icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/questionMark.png");
    icon.x = 634;
    icon.y = 980;
    //no highlight and draw offset is on for all map sprites
    icon.noHighlight = true;
    icon.offset = true;
    icon.scaleX = 0.1;
    icon.scaleY = 0.1;

    icon_manager.icons[0] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/mapBattle.png");
    icon.x = 32;
    icon.y = 32;
    icon.scaleX = 3;
    icon.scaleY = 3;

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

    icon.sprite = icon_manager.icons[4].sprite;
    icon.noHighlight = false;
    icon.scaleX = 30;
    icon.scaleY = 30;
    icon.x = 32;
    icon.y = 32;

    icon_manager.icons[9] = icon;
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
    self->iconType = (int)icon;
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
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * ent->pixel.x,ent->scale.y * ent->pixel.y);


    if(icon < ChoiceBattle)
        ent->type = MapIcon;
    else if (icon < EventShrine)
        ent->type = ChoiceIcon;
    else
        ent->type = EventIcon;

    return ent;
}

void icon_get_world(World *w)
{
    if(!w) return;

    world = w;
}

void icon_get_player(Entity *p)
{
    if(!p) return;

    player = p;
}
void icon_think(Entity *self)
{
    if(!self) return;

    //IconInfo info = icon_manager.icons[(Icons)self->data];
    //slog("%i",(int)self->data);
}

void icon_update(Entity *self)
{
    if(!self) return;

}

//starts event based on event type
void event_start(EventType type)
{
    state = Event;
    

    //slog("%i", state);

    switch(type)
    {
        case None:
        slog("Event start fail");
        return;
        case Explore:
        slog("Explore event");
        return;
        case Shop:
        slog("Shop event");
        return;
        case Shrine:
        slog("Shrine event");
        gfc_list_append(tempIcons,icon_new(vector2d(1220,480), EventShrine));
    }

    grace = SDL_GetTicks() + graceInterval;

    event = type;
}

//closes current event, updates state and map
void event_close()
{
    int i;
    Entity *ent;
    while(gfc_list_get_count(tempIcons) > 0)
    {
        i = gfc_list_get_count(tempIcons) - 1;
        ent = gfc_list_get_nth(tempIcons,i);

        if(ent) entity_free(ent);

        gfc_list_delete_last(tempIcons);
    }

    state = Choice;
    

    if(level < 5)
    {
        ent = gfc_list_get_nth(world->icons,level + 3);
        switch(event)
        {
            case None:
            slog("failure");
            break;
            case Explore:
            icon_set_sprite(ent,MapExplore);
            break;
            case Shop:
            icon_set_sprite(ent, MapShop);
            break;
            case Shrine:
            icon_set_sprite(ent, MapShrine);
            break;
        }

        level++;
    }
    event = None;
}
void icon_leftClick(Entity *self)
{
    if(!self) return;

    Icons icon;
    int playerHealthGain;

    icon = self->iconType;


    switch(icon)
    {
        //skip anything not clickable
        case ChoiceBattle:
        slog("Battle Click");
        return;

        case ChoiceExplore:
        slog("Explore Click");
        return;

        case ChoiceShrine:
        event_start(Shrine);
        return;

        case EventShrine:
        //adds cooldown before you can click to end event
        if(grace > SDL_GetTicks()) return;

        playerHealthGain = (int)(player->healthMax * 0.33);
        if(player->health + playerHealthGain > player->healthMax)
        {
            player->health = player->healthMax;
        }
        else
        {
            player->health = player->health + playerHealthGain;
        }
        event_close();
        return;

        default:
        return;
    }
}