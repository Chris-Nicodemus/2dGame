#include "simple_logger.h"
#include "gfc_audio.h"
#include "world.h"
#include "entity.h"
#include "icon.h"
#include "font.h"

extern State state;
static Vector2D choice1;
static Vector2D choice2;
static Vector2D choice3;

static Vector2D map1;
static Vector2D map2;
static Vector2D map3;
static Vector2D map4;
static Vector2D map5;

World *world_new()
{
    World *world;

    world = gfc_allocate_array(sizeof(World),1);
    if(!world)
    {
        slog("failed to allocate world");
        return NULL;
    }
    choice1 = vector2d(150,100);
    choice2 = vector2d(900,100);
    choice3 = vector2d(1675,100);

    map1 = vector2d(920,1275);
    map2 = vector2d(1525,1260);
    map3 = vector2d(1660,650);
    map4 = vector2d(1190,1020);
    map5 = vector2d(650,330);

    world->icons = gfc_list_new();
    world->bgs = gfc_list_new();

    world->background = gf2d_sprite_load_image("images/backgrounds/cityDay.png");
    world->name = "images/backgrounds/cityDay.png";
    world->scale = vector2d(1.04166667,1.111111111);

    //choice icons 0-2
    gfc_list_append(world->icons, icon_new(choice1,ChoiceBattle));
    gfc_list_append(world->icons, icon_new(choice2,ChoiceExplore));
    gfc_list_append(world->icons, icon_new(choice3,ChoiceShrine));

    //map icons 3-7
    gfc_list_append(world->icons, icon_new(map1, Question));
    gfc_list_append(world->icons, icon_new(map2, Question));
    gfc_list_append(world->icons, icon_new(map3, Question));
    gfc_list_append(world->icons, icon_new(map4, Question));
    gfc_list_append(world->icons, icon_new(map5, Question));
    

    //bg sprites
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/cityDay.png"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/cityNight.png"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/stoneWall.jpg"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/MapFullCentered.jpg"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/cityOpening.png"));
    return world;
}

void world_free(World *world)
{
    if(!world) return;

    if(world->name)
    free(world->name);

    gfc_list_delete(world->icons);

    gfc_list_delete(world->bgs);

    gf2d_sprite_free(world->background);
}

void world_get_state(World *world)
{
    if(!world) return;

    //slog("%i",state);
    switch(state)
    {
        case Combat:
            if(strcmp(world->name,"images/backgrounds/cityDay.png"))
            {
                world->name = "images/backgrounds/cityDay.png";
                world->background = gfc_list_get_nth(world->bgs, 0);
                world->scale = vector2d(1.04166667,1.111111111);
            }
            return;
        case Event:
            //slog("getting here");
            if(strcmp(world->name,"images/backgrounds/cityNight.png"))
            {
                world->name = "images/backgrounds/cityNight.png";
                world->background = gfc_list_get_nth(world->bgs, 1);
                world->scale = vector2d(1.04166667,1.111111111);   
            }
            return;
        case Choice:
            if(strcmp(world->name,"images/backgrounds/stoneWall.jpg"))
            {
                world->name = "images/backgrounds/stoneWall.jpg";
                world->background = gfc_list_get_nth(world->bgs, 2);
                world->scale = vector2d(2.684563758,1.610738255);   
            }
            return;
        case Map:
            //slog("getting here");
            if(strcmp(world->name,"images/backgrounds/MapFullCentered.jpg"))
            {
                world->name = "images/backgrounds/MapFullCentered.jpg";
                world->background = gfc_list_get_nth(world->bgs, 3);
                world->scale = vector2d(1.111111111,1);   
            }
            return;
        case MainMenu:
            if(strcmp(world->name, "images/backgrounds/cityOpening.png"))
            {
                world->name = "images/backgrounds/cityOpening.png";
                world->background = gfc_list_get_nth(world->bgs, 4);
                world->scale = vector2d(1.04166667,1.111111111);   
            }
            return;
        case Multiplayer:
            if(strcmp(world->name,"images/backgrounds/cityNight.png"))
            {
                world->name = "images/backgrounds/cityNight.png";
                world->background = gfc_list_get_nth(world->bgs, 1);
                world->scale = vector2d(1.04166667,1.111111111);   
            }
            return;
        default:
            slog("state not implemented");
            return;
    }
}

void world_draw(World *world)
{
    if(!world) return;

    world_get_state(world);

    if(world->background)
    {
        gf2d_sprite_draw(world->background,vector2d(0,0),&world->scale,NULL,NULL,NULL,NULL,0);
    }
}

void world_open_main_menu()
{
    state = MainMenu;

    Mix_Music *opening = Mix_LoadMUS("audio/opening.mp3");
    if(opening)
    {
        Mix_PlayMusic(opening, -1);
        Mix_VolumeMusic(32);
        //slog("music volume: %i",Mix_VolumeMusic(-1));
    }
    else
    {
        slog("music fail");
    }
    text_new("Play", FS_Large, vector2d(1875,750),vector2d(5,5), GFC_COLOR_WHITE, 0, TT_MainMenu_Button);

    text_new("PvP", FS_Large, vector2d(1875,850),vector2d(5,5), GFC_COLOR_WHITE, 0, TT_MainMenu_Button);

    text_new("Challenge", FS_Large, vector2d(1875,950),vector2d(3,3), GFC_COLOR_WHITE, 0, TT_MainMenu_Button);
}