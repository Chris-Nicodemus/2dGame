#include "simple_logger.h"
#include "gfc_list.h"
#include "gf2d_draw.h"
#include "font.h"
#include "icon.h"
#include "enemy.h"
#include "player.h"


extern State state;
extern Bool turn;
extern Bool target;
extern List *targets;
extern int targetsNeeded;

typedef struct
{
    List  *enemy_list;
    Uint8   enemy_max;  
}EnemyManager;

typedef enum
{
    NoAction,
    Attack,
    Block,
    Mix,
    Heal
}EnemyActions;


static EnemyManager enemy_manager = {0};
void bug_think(Entity *self);
void bug_update(Entity *self);
void enemy_leftClick(Entity *self);
void enemy_draw(Entity *self);
void enemy_free(Entity *self);

void enemy_system_close()
{
    if(enemy_manager.enemy_list)
    free(enemy_manager.enemy_list);

    memset(&enemy_manager,0,sizeof(EnemyManager));

    slog("Entity System Close");
}

void enemy_system_init(Uint8 enemyMax)
{
    if(enemy_manager.enemy_list)
    {
        slog("cannot have more than one enemy manager");
        return;
    }
    
    if(!enemyMax)
    {
        slog("cannot allocate 0 entities!");
        return;
    }

    enemy_manager.enemy_list = gfc_list_new();

    if(!enemy_manager.enemy_list)
    {
        slog("failed to allocate global enemy list");
        return;
    }

    enemy_manager.enemy_max = enemyMax;
    slog("enemy system start");
    atexit(enemy_system_close);
}

Entity *enemy_new(Vector2D pos, EnemyType type)
{
    if(gfc_list_get_count(enemy_manager.enemy_list) + 1 > enemy_manager.enemy_max)
    {
        slog("Too many enemies. Cancelling spawn");
        return NULL;
    }

    if(type == NotEnemy)
    {
        slog("Cannot spawn non enemy");
        return NULL;
    }

    Entity *ent = entity_new();

    if(!ent)
    {
        slog("failed to make empty entity");
        return NULL;
    }

    switch (type)
    {
    case Bug:
        ent->sprite = gf2d_sprite_load_all("images/space_bug.png",128,128,16,0);
        ent->frame = 0;
        ent->enemyType = type;
        ent->scale = vector2d(2.5,2.5);
        ent->pixel = vector2d(128,128);
        ent->frameStart = 88;
        ent->frame = ent->frameStart;
        ent->frameEnd = 104;
        ent->think = bug_think;
        ent->update = bug_update;
        ent->healthMax = (int) (gfc_random() * 13) + 25;
        ent->health = ent->healthMax;
        break;
    
    default:
        slog("Entity failed because type not yet implemented");
        return NULL;
        break;
    }

    //general stuff here
    ent->draw = enemy_draw;
    ent->leftClick = enemy_leftClick;
    ent->free = enemy_free;
    ent->drawOffsetX = -ent->pixel.x * ent->scale.x * 0.5;
    ent->drawOffsetY = -ent->pixel.y * ent->scale.y * 0.5;
    ent->bounds = gfc_rect(ent->position.x + ent->drawOffsetX,ent->position.y + ent->drawOffsetY,ent->scale.x * ent->pixel.x,ent->scale.y * ent->pixel.y);
    ent->position = pos;
    ent->type = Enemy;
    gfc_list_append(enemy_manager.enemy_list,ent);
    return ent;
}

void enemy_free(Entity *self)
{
    if(!self) return;

    //delete index on the enemy array
    gfc_list_delete_data(enemy_manager.enemy_list, self);
}

void enemy_draw(Entity *self)
{
    if(!self) return;

    float healthRatio;
    Rect damage,health;
    Color red = gfc_color8(100,0,0,255);
    Color green = gfc_color8(50,208,0,255);
    Color blue = gfc_color8(84, 195, 212, 255);
    float barX,barY;
    float barLength, barWidth = 32;
    char *text = (char *)malloc(sizeof(char) * (10));
    int blockOffset;


    if(!self) return;

    barLength = 128 * self->scale.x;
    healthRatio = (float)self->health / (float)self->healthMax;
            
    barX = self->position.x - (64 * self->scale.x);
    barY = self->position.y + (64 * self->scale.y);

    damage = gfc_rect(barX,barY, barLength,barWidth);
    health = gfc_rect(barX,barY,barLength * healthRatio, barWidth);
            
    gf2d_draw_rect_filled(damage,red);
    gf2d_draw_rect_filled(health,green);

    sprintf(text,"%i / %i",self->health,self->healthMax);

    font_draw_text(text,FS_Small, gfc_color(1,1,1,1), vector2d(self->position.x - (self->pixel.x * 0.5 * self->scale.x), self->position.y + (self->pixel.y * 0.5 * self->scale.y) + (barWidth *0.33)),vector2d(1.5,1.5));

    if(self->block)
    {
        gf2d_draw_rect(damage,blue);
        sprintf(text,"%i",self->block);
        if(self->block < 10)
        blockOffset = 30;
        else
        blockOffset = 40;

        font_draw_text(text,FS_Small, GFC_COLOR_CYAN, vector2d(self->position.x - (self->pixel.x * 0.5 * self->scale.x) - blockOffset, self->position.y + (self->pixel.y * 0.5 * self->scale.y) + (barWidth *0.33)),vector2d(1.5,1.5));
    }

    switch(self->action)
    {
        case Attack:
        sprintf(text,"%i", self->actionStrength);
        font_draw_text(text,FS_Medium,gfc_color(1,0,0,1),vector2d(self->position.x, self->position.y - (0.5 * self->scale.y * self->pixel.y)),vector2d(3,3));
        break;

        case Block:
        sprintf(text,"%i", self->actionStrength);
        font_draw_text(text,FS_Medium,gfc_color(0,0,1,1),vector2d(self->position.x, self->position.y - (0.5 * self->scale.y * self->pixel.y)),vector2d(3,3));

        default:
        //do nothing
        break;
    }
}

void enemy_leftClick(Entity *self)
{
    if(!self) return;

    if(!target)
    {   
        slog("target false");
        return;
    }

    //return if enough targets
    //slog("happening");
    if(gfc_list_get_count(targets) >= targetsNeeded) return;

    //return if already in list
    if(gfc_list_get_item_index(targets, self) != -1) return;

    gfc_list_append(targets, self);
}

void bug_think(Entity *self)
{
    if(!self) return;

    //slog("%i", (int)self->frame);

}

void bug_update(Entity *self)
{
    if(!self) return;
}

int enemy_get_count()
{
    return gfc_list_get_count(enemy_manager.enemy_list);
}

void enemy_damage(Entity *victim, Entity *self, int damage, DamageType dammageType)
{
    int i;
    if(!victim) return;

    if(!self) return;

    if(victim->block)
    {

        if(victim->block < damage)
        {
            damage -= victim->block;
            victim->block = 0;
        }

        if(victim->block > damage)
        {
            victim->block = victim->block - damage;
            damage = 0;
        }
    }

    if(victim->health <= damage)
    {
        victim->health = 0;
        i = gfc_list_get_item_index(enemy_manager.enemy_list,victim);
        gfc_list_delete_nth(enemy_manager.enemy_list, i);
        entity_free(victim);

        if(gfc_list_get_count(enemy_manager.enemy_list) == 0)
        icon_combat_win(entity_get_player());
        return;
    }

    victim->health = victim->health - damage;
}

void decide_intent(Entity *self)
{
    if(!self) return;

    EnemyType type = self->enemyType;
    int i;

    switch (type)
    {
        case Bug:
        i = (int) (gfc_random() * 2) + 1;
            switch(i)
            {
                case Attack:
                self->action = i;
                self->actionStrength = (int) (gfc_crandom() * 6) + 4;
                return;
                case Block:
                self->action = i;
                self->actionStrength = (int) (gfc_crandom() * 6) + 4;
                return;
                default:
                slog("Action failure");
                return;
            }
        break;


        default:
        slog("enemy not implemented");
        return;
    }
}

void enemy_intent()
{
    int i, j;
    j = gfc_list_get_count(enemy_manager.enemy_list);

    for(i = 0; i < j; i++)
    {
        decide_intent(gfc_list_get_nth(enemy_manager.enemy_list,i));
    }
}

void do_action(Entity *self)
{
    if(!self) return;

    switch(self->action)
    {
        case Attack:
        player_damage(entity_get_player(),self,self->actionStrength,0);
        self->action = 0;
        self->actionStrength = 0;
        return;
        case Block:
        self->block = self->block + self->actionStrength;
        self->action = 0;
        self->actionStrength = 0;
        return;

        default:
        slog("invalid action");
        return;
    }
}

void enemy_act()
{
    int i, j;
    j = gfc_list_get_count(enemy_manager.enemy_list);

    for(i = 0; i < j; i++)
    {
        do_action(gfc_list_get_nth(enemy_manager.enemy_list,i));
    }
}

void enemy_upkeep()
{
    int i, j;
    j = gfc_list_get_count(enemy_manager.enemy_list);
    Entity *enemy;
    for(i = 0; i < j; i++)
    {
        enemy = gfc_list_get_nth(enemy_manager.enemy_list,i);

        if(!enemy) continue;

        if(enemy->block) enemy->block = 0;
    }
}