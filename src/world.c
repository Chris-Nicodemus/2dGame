#include "simple_logger.h"
#include "world.h"
#include "entity.h"

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

    return world;
}

void world_free(World *world)
{
    if(!world) return;

    while(gfc_list_get_count(world->icons) > 0)
    {
        gfc_list_delete_last(world->icons);
    }

    while(gfc_list_get_count(world->bgs) > 0)
    {
        gfc_list_delete_last(world->bgs);
    }

    gfc_list_delete(world->icons);

    gf2d_sprite_free(world->background);
}

void world_draw(World *world)
{
    if(!world) return;

    if(world->background)
    {
        gf2d_sprite_draw(world->background,vector2d(0,0),&world->scale,NULL,NULL,NULL,NULL,0);
    }
}