#include "simple_logger.h"
#include "gfc_audio.h"
#include "gfc_list.h"
#include "font.h"
#include "icon.h"
#include "card.h"
#include "enemy.h"
#include "player.h"
#include "consumable.h"
#include "particles.h"


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
List *tempIcons;
static World *world;
static Entity *player;

extern State state;
extern EventType event;
extern int level;

extern Bool target;
extern Bool turn;
extern Bool showShop;

extern Sprite *goldIcon;

Uint32 graceInterval = 100;
Uint32 grace;

void icon_think(Entity *self);
void icon_update(Entity *self);
void icon_leftClick(Entity *self);
void end_turn_draw(Entity *self);
//void icon_rightClick(Entity *self);

Sound *button;

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

    button = gfc_sound_load("audio/system-notification.mp3",1,0);

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
    goldIcon = icon.sprite;

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

    icon.sprite = gf2d_sprite_load_image("images/icons/chest.png");
    icon.scaleX = 15;
    icon.scaleY = 15;

    icon_manager.icons[10] = icon;

    icon.sprite = gf2d_sprite_load_image("images/icons/button.png");
    icon.scaleX = 0.5;
    icon.scaleY = 0.5;
    icon.x = 480;
    icon.y = 480;

    icon_manager.icons[11] = icon;
    
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

    if(icon == EndTurn)
    self->draw = end_turn_draw;
    else
    self->draw = NULL;
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
    else if (icon < EndTurn)
        ent->type = EventIcon;
    else
        ent->type = CombatButton;
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

char *icon_get_event_text()
{
    char *text;

    switch(event)
    {
        case Shrine:
        text = "Rest at the shrine";
        break;
        
        case Chest:
        text = "A chest! What might it hold?";
        break;

        case ChestOpen:
        text = "A card! Do you accept it?";
        break;

        default:
        text = "Event fail";
    }

    return text;
}

void event_set(EventType type)
{
    int i, x, y, xOff, yOff;
    Entity *ent;

    switch (type)
    {
    case Chest:
        event = Chest;
        gfc_list_append(tempIcons,icon_new(vector2d(1220,480), EventChest));
        text_new(icon_get_event_text(),FS_Medium, vector2d(835,155), vector2d(2,2), gfc_color(1,1,1,1), 0, TT_Event);
        break;
    case ChestOpen:
        event = ChestOpen;
        text_clear_perms(NULL);
        text_new(icon_get_event_text(),FS_Medium, vector2d(835,155), vector2d(2,2), gfc_color(1,1,1,1), 0, TT_Event);
        text_new("Decline",FS_Medium, vector2d(1830,1125), vector2d(4,4), gfc_color(1,1,1,1), 0, TT_EventChoice);
        break;
    case ChestFight:
        while(gfc_list_get_count(tempIcons) > 0)
        {
            i = gfc_list_get_count(tempIcons) - 1;
            ent = gfc_list_get_nth(tempIcons,i);

            if(ent) entity_free(ent);

            gfc_list_delete_last(tempIcons);
        }
        entity_clear_gifts();
        event = ChestFight;
        text_clear_perms(NULL);
        text_new("Ambush!",FS_Large,vector2d(1000,650),vector2d(3,3),GFC_COLOR_WHITE, 1500, TT_Event);
        gfc_list_append(tempIcons,icon_new(vector2d(1950,920),EndTurn));
        ent = enemy_new(vector2d(1600,740),Bug);
        ent->gold = ent->gold + 50;
        player_combat_start(entity_get_player());
        break;
    case Shop:
        event = Shop;
        x = 1165;
        y = 230;
        xOff = 189 + 60;
        yOff = 245.7 + 32 + 50;

        text_new("Leave",FS_Medium,vector2d(300,1220),vector2d(4,4),GFC_COLOR_WHITE,0,TT_EventChoice);
        text_new("Shop",FS_Large,vector2d(1490,60),vector2d(3,3),GFC_COLOR_WHITE, 0, TT_Event);

        for(i = 0; i < 12; i++)
        {
            ent = card_shop_new();
            gfc_list_append(tempIcons,ent);
            ent->position = vector2d(x, y);

            x += xOff;

            if((i + 1) % 4 == 0)
            {
                x = 1165;
                y += yOff;
            }
        }

        x = 1165 + 45;
        for(int i = 0; i < 4; i++)
        {
            gfc_list_append(tempIcons,consumable_new(vector2d(x,y),consumable_get_random(),false));

            x += xOff;
        }
        break;
    default:
        break;
    }
    
}


//starts event based on event type
void event_start(EventType type)
{
    state = Event;
    int i;

    //slog("%i", state);

    switch(type)
    {
        case None:
        slog("Event start fail");
        return;
        case Explore:
        slog("Explore event");
        i = (int)(gfc_random() * 2) + 2;
        //slog("%i",i);
        event_set(i);
        break;
        case Shop:
        slog("Shop event");
        break;
        case Shrine:
        slog("Shrine event");
        gfc_list_append(tempIcons,icon_new(vector2d(1220,480), EventShrine));
        event = Shrine;
        break;

        default:
        slog("Event start fail");
        return;
    }

    grace = SDL_GetTicks() + graceInterval;
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

    if(level < 5)
    {   
        ent = gfc_list_get_nth(world->icons,level + 3);
        if(state == Combat)
        icon_set_sprite(ent, MapBattle);
        
        if(event != 0)
        {
            switch(event)
            {
                case None:
                slog("failure");
                break;
                case Shop:
                icon_set_sprite(ent, MapShop);
                break;
                case Shrine:
                icon_set_sprite(ent, MapShrine);
                break;
                default:
                icon_set_sprite(ent,MapExplore); //any other event will be explore
                break;
            }
        }
        level++;
    }
    state = Choice;
    event = None;

    text_clear_perms(NULL);
    particle_clear();
}


void icon_leftClick(Entity *self)
{
    if(!self) return;

    Icons icon;
    int playerHealthGain;
    Entity *ent; //general use

    icon = self->iconType;


    switch(icon)
    {
        //skip anything not clickable
        case ChoiceBattle:
        //slog("Battle Click");
        state = Combat;
        gfc_list_append(tempIcons,icon_new(vector2d(1950,920),EndTurn));
        ent = enemy_new(vector2d(1600,740),Bug);
        player_combat_start(entity_get_player());
        particle_clear();
        return;

        case ChoiceExplore:
        //slog("Explore Click");
        event_start(Explore);
        particle_clear();
        return;

        case ChoiceShrine:
        state = Event;
        event_start(Shrine);
        particle_clear();
        return;

        case EventShrine:
        //adds cooldown before you can click to end event
        if(grace > SDL_GetTicks()) break;

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

        case EventChest:
        if(grace > SDL_GetTicks()) break;
        entity_free(self);
        event_set(ChestOpen);
        ent = card_new(card_get_random(),NULL);
        ent->gift = true;
        ent->scale = vector2d(.36,.36);
        ent->position = vector2d(1190,470);
        //event_close();
        return;

        case EndTurn:
        if(target) return;
        
        gfc_sound_play(button,0,1,0,0);
        if(state == Combat || event == ChestFight)
        {
            if(!entity_get_player())
            {
                slog("player not found");
                return;
            }
            player_end_turn(entity_get_player());
            return;
        }
        else
        {
            player_multi_end_turn();
        }
        default:
        return;
    }
}

void end_turn_draw(Entity *self)
{
    if(!self) return;

    font_draw_text("End Turn",FS_Large,gfc_color(1,1,1,1),vector2d(self->position.x + (self->pixel.x * self->scale.x * 0.25) - 25,self->position.y + (self->pixel.y * self->scale.y * 0.5) - 10), vector2d(1.25,1.25));
}

void icon_combat_win(Entity *player)
{
    if(!player) return;

    player->airborne = false;
    player->block = 0;
    //put the hand back in the deck
    player_discard_hand(player);
    player_shuffle(player);

    event_close();
}