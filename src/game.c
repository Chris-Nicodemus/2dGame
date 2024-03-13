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
#include "font.h"
#include "world.h"
#include "entity.h"
#include "player.h"
#include "card.h"
#include "icon.h"

Bool leftClicked;
Bool rightClicked;
State oldState;
State state = Choice;
EventType event = None;
int level = 0;

List *deckDisplay;
Bool showDeck = false;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    //int i; general use
    const Uint8 * keys;
    //Sprite *sprite;
    World *world;
    
    int mx,my;
    Uint32 mButton;
    
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,100,255,200);
    
    deckDisplay = gfc_list_new();

    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    gfc_input_init("gfc/sample_config/input.cfg");
    
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        2400,
        1440,
        2400,
        1440,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(24);
    font_init();
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    icon_init();
    
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    world = world_new();
    icon_get_world(world);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    /*main game loop*/

    Entity *player = player_new(vector2d(690,740));
    icon_get_player(player);
    FILE *deck = fopen("config/deck.json","r");
    if(get_file_Size(deck) == 0)
    {
        player_new_deck(player);
        slog("deck empty making new deck");
    }
    else
    {
        player_load_deck(player);
        slog("loading deck");
    }
    fclose(deck);

    player_shuffle(player);
    //player_draw(player,10);

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
            

            if(mButton == SDL_BUTTON_LEFT && !leftClicked)
            {
                //slog("x: %i, y: %i",mx,my);
                leftClicked = true;
            }
            else if(leftClicked && mButton != SDL_BUTTON_LEFT)
            {
                leftClicked = false;
            }
            
            //slog("mbutton: %i", mButton);
            if(mButton == 4 && !rightClicked)
            {
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

            //font_draw_text("Testing!",FS_Medium,GFC_COLOR_LIGHTBLUE,vector2d(20,30),vector2d(3,3));

            if(event != None)
            {
                //slog("%i",event);
                font_draw_text(icon_get_event_text(),FS_Medium,gfc_color(1,1,1,1),vector2d(20,30),vector2d(2,2));
            }

            if(showDeck)
            {
                player_show_deck(player);
            }
            else if(!showDeck && gfc_list_get_count(deckDisplay) > 0)
            {
                //slog("attempting delete");
                player_show_deck_close();
            }

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if(gfc_input_command_pressed("map"))
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

        if(gfc_input_command_pressed("deck"))
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
