#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "gfc_audio.h"

#include "font.h"
#include "world.h"
#include "custom_deck.h"
#include "entity.h"
#include "player.h"
#include "card.h"
#include "icon.h"
#include "enemy.h"
#include "consumable.h"


Bool leftClicked;
Uint32 leftClickCooldown;
Uint32 clickCooldownInterval = 150; //used for both left and right click
Bool rightClicked;
Uint32 rightClickCooldown;

State oldState;
State state;
EventType event = None;
int level = 0;
Bool turn = false;

//stuff for player to hit enemies
Bool target;
List *targets;
Uint8 targetsNeeded;

List *deckDisplay;
Bool showDeck = false;
Entity *usedCard;
World *world;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int i; //general use
    const Uint8 * keys;
    Bool custom = false;
    //Sprite *sprite;
    
    int mx,my;
    Uint32 mButton;
    
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    deckDisplay = gfc_list_new();
    targets = gfc_list_new();
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    gfc_input_init("gfc/sample_config/input.cfg");
    
    slog("---==== BEGIN ====---");

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--custom") == 0)
        {
            slog("launching deck editor custom map");
            custom = true;
        }
    }

    if(custom)
    custom_deck_write();

    gf2d_graphics_initialize(
        "gf2d",
        2400,
        1440,
        2400,
        1440,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(24);
    font_init(512);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    icon_init();
    enemy_system_init(24);

    gfc_audio_init(100,64,4,10,1,0);
    
    //slog("x: %f, y: %f", play->textSize.x, play->textSize.y);
    
    /*Mix_Music *opening = Mix_LoadMUS("audio/opening.mp3");
    if(opening)
    {
        Mix_PlayMusic(opening, -1);
        Mix_VolumeMusic(32);
        slog("music volume: %i",Mix_VolumeMusic(-1));
    }
    else
    {
        slog("music fail");
    }*/

    world_open_main_menu();
    

    SDL_ShowCursor(SDL_DISABLE);
    
    
    world = world_new();
    icon_get_world(world);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    /*main game loop*/

    Entity *player = player_new(vector2d(690,740));
    
    icon_get_player(player);

    consumable_init();

    FILE *deck = fopen("config/deck.json","r");
    if(get_file_Size(deck) == 0)
    {
        player_new_deck(player);
        slog("deck empty making new deck");
    }
    else
    {
        player_load_deck(player, "config/deck.json");
        slog("loading deck");
    }
    fclose(deck);
    
    consumable_new(vector2d(0,0), "shield", true);
    consumable_new(vector2d(0,0), "sky", true);
    consumable_new(vector2d(0,0), "energy", true);
    //consumable_new(vector2d(0,0),"fire",false);
    //consumable_new(vector2d(0,0), "fire", true);

    while(!done)
    {
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        
        /*update things here*/
        mButton = SDL_GetMouseState(&mx,&my);

        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        entity_system_think();
        entity_system_update();
            
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,vector2d(0,0));


            world_draw(world);

            //entities middle
            entity_system_draw();

            entity_highlight_all();

            //gf2d_draw_rect_filled(gfc_rect(player->position.x,player->position.y,5,5),gfc_color(1,0,0,1));
            
            text_update_all();

            text_draw_all();

            if(mButton == SDL_BUTTON_LEFT && !leftClicked)
            {
                
                leftClicked = true;
                leftClickCooldown = SDL_GetTicks() + clickCooldownInterval;
                //state = Event;
                //player_draw(player,1);
            }
            else if(leftClicked && mButton != SDL_BUTTON_LEFT && SDL_GetTicks() > leftClickCooldown)
            {
                leftClicked = false;
            }
            
            //slog("mbutton: %i", mButton);
            if(mButton == 4 && !rightClicked)
            {
                slog("x: %i, y: %i",mx,my);
                rightClicked = true;
                //slog("game happening");
            }
            else if(rightClicked && mButton != 4)
            {
                rightClicked = false;
            }
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

            if(showDeck)
            {
                player_show_deck(player);
            }
            else if(!showDeck && gfc_list_get_count(deckDisplay) > 0)
            {
                //slog("attempting delete");
                player_show_deck_close();
            }

            if(state == Combat || event == ChestFight)
            {
                if(target)
                {
                    i = targetsNeeded - gfc_list_get_count(targets);
                    switch(i)
                    {
                        case 1:
                        font_draw_text("Select a target!",FS_Medium,gfc_color(1,1,1,1),vector2d(1000,60),vector2d(3,3));
                        break;
                        //2 does same as 3
                        case 2:
                        case 3:
                        font_draw_text("Select targets!",FS_Medium,gfc_color(1,1,1,1),vector2d(1050,60),vector2d(3,3));
                        break;
                        //do nothing if error
                        default:
                        break;
                    }

                    if(targetsNeeded == gfc_list_get_count(targets) || gfc_list_get_count(targets) == enemy_get_count())
                    {
                        player_do_action(player,usedCard);
                        targetsNeeded = 0;
                        target = false;
                        while(gfc_list_get_count(targets) > 0)
                        {
                            gfc_list_delete_last(targets);
                        }
                    }
                }
            }

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if(gfc_input_command_pressed("map") && state != MainMenu)
        {
            if(state == Map)
            {
                state = oldState;
            }
            else
            {
                oldState = state;
                state = Map;
            }
        }

        if(gfc_input_command_pressed("deck") && state != MainMenu)
        {
            if(!showDeck)
            showDeck = true;
            else
            showDeck = false;
        }

        if (keys[SDL_SCANCODE_ESCAPE])
        {
            player_save_deck(player);
            done = 1; // exit condition
        }
        if (keys[SDL_SCANCODE_Q])
        {
            FILE *deck = fopen("config/deck.json","w");
            fclose(deck);
            done = 1; //secondary exit condition
        } 
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_clear_all(NULL);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
