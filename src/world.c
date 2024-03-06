#include "simple_logger.h"
#include "world.h"
#include "entity.h"

extern State state;

World *world_new()
{
    World *world;

    world = gfc_allocate_array(sizeof(World),1);
    if(!world)
    {
        slog("failed to allocate world");
        return NULL;
    }

    world->icons = gfc_list_new();
    world->bgs = gfc_list_new();

    world->background = gf2d_sprite_load_image("images/backgrounds/cityDay.png");
    world->name = "images/backgrounds/cityDay.png";
    world->scale = vector2d(1.04166667,1.111111111);

    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/cityDay.png"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/cityNight.png"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/stoneWall.jpg"));
    gfc_list_append(world->bgs,gf2d_sprite_load_image("images/backgrounds/MapFullCentered.jpg"));
    return world;
}

void world_free(World *world)
{
    if(!world) return;

    free (world->name);

    gfc_list_delete(world->icons);

    gfc_list_delete(world->bgs);

    gf2d_sprite_free(world->background);
}

void world_get_state(World *world)
{
    if(!world) return;

    
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