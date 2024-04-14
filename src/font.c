#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_audio.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "font.h"
#include "entity.h"
#include "icon.h"
#include "player.h"


typedef struct
{
    char* text;
    FontStyles style;
    Color color;
    SDL_Texture *texture;
    Vector2D size;
    Uint32 timestamp;
    
}FontCache;

typedef struct
{
    List *fonts;
    List *textures;
}FontManager;

typedef struct 
{
    Text *text_list;
    Uint32 text_max;
} TextManager;

static FontManager font_manager = {0};
static TextManager text_manager = {0};

extern Bool leftClicked;
extern Bool rightClicked;
extern State state;
extern List *tempIcons;

//do these next
void text_main_menu_button_update(Text *self);
void text_main_menu_button_moused(Text *self);
void text_play_left_click(Text *self);
void text_pvp_left_click(Text *self);

void font_close()
{
    //other stuff to stop
    while(gfc_list_get_count(font_manager.fonts) > 0)
    {
        TTF_CloseFont(gfc_list_get_nth(font_manager.fonts,gfc_list_get_count(font_manager.fonts) - 1));
        gfc_list_delete_last(font_manager.fonts);
    }

    
    gfc_list_delete(font_manager.fonts);
    memset(&font_manager,0,sizeof(FontManager));

    if(text_manager.text_list)
    free(text_manager.text_list);

    memset(&text_manager,0,sizeof(TextManager));

    TTF_Quit();
}

void font_init(Uint32 maxText)
{
    TTF_Font *font;
    int i;

    if(TTF_Init() != 0)
    {
        slog("failed to start font");
        return;
    }

    font_manager.fonts = gfc_list_new();
    font_manager.textures = gfc_list_new();

    for(i = 0; i < FS_MAX; i++)
    {
        font = TTF_OpenFont("font/Arkitech Medium.ttf", 10 + (i * 4));

        if(!font)
        {
            slog("failed to add font: font/Arkitech Medium.ttf");
            continue;
        }

        gfc_list_append(font_manager.fonts,font);
    }

    if(text_manager.text_list)
    {
        slog("cannot have more than one text manager");
        return;
    }
    
    if(!maxText)
    {
        slog("cannor allocate 0 text!");
        return;
    }

    text_manager.text_list = gfc_allocate_array(sizeof(Text),maxText);
    if(!text_manager.text_list)
    {
        slog("failed to allocate global text list");
        return;
    }

    text_manager.text_max = maxText;

    atexit(font_close);    
}

void font_cache_free(FontCache *cache)
{
    if(!cache) return;

    if(cache->texture) SDL_DestroyTexture(cache->texture);
    
    if(cache->text) free(cache->text);
    
    free(cache);
}

void font_cleanup()
{
    Uint32 time = SDL_GetTicks();
    FontCache *cache;
    int i;

    for(i = 0; i < gfc_list_get_count(font_manager.textures); i++)
    {
        cache = gfc_list_get_nth(font_manager.textures,i);
        if(!cache) continue;

        if(time > cache->timestamp + 100)
        {
            font_cache_free(cache);
            gfc_list_delete_nth(font_manager.textures,i);
            slog("deleted");
            i--;
        }
    }
}

void font_add_texture(const char* text, FontStyles style, Color color, SDL_Texture *texture, Vector2D size)
{
    FontCache *cache;
    size_t length;

    if(!text || !texture) return;

    cache = gfc_allocate_array(sizeof(FontCache),1);
    if(!cache) return;

    length = strlen(text) + 1;
    cache->text = gfc_allocate_array(sizeof(char),length);
    strcpy(cache->text,text);
    vector2d_copy(cache->size,size);
    cache->color = color;
    cache->style = style;
    cache->texture = texture;
    cache->timestamp = SDL_GetTicks();

    gfc_list_append(font_manager.textures,cache);
}

FontCache *font_get_texture(const char* text, FontStyles style, Color color)
{
    FontCache *cache;
    int i;

    if(!text) return NULL;

    for(i = 0; i < gfc_list_get_count(font_manager.textures); i++)
    {
        cache = gfc_list_get_nth(font_manager.textures,i);

        if(!cache) continue;

        if(strcmp(cache->text,text) != 0) continue;

        if(cache->style != style) continue;

        if(gfc_color_cmp(cache->color,color) == 0) continue;

        return cache;
    }

    return NULL;
}

void font_draw_text(const char* text, FontStyles style, Color color, Vector2D position, Vector2D scale)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    FontCache *cache;

    cache = font_get_texture(text,style,color);
    if(cache)
    {
        //slog("getting here");
        cache->timestamp = SDL_GetTicks();
        //texture = cache->texture;

        rect.x = position.x;
        rect.y = position.y;

        if((!scale.x) || (!scale.y))
        {
            rect.w = cache->size.x;
            rect.h = cache->size.y;
        }
        else
        {
            rect.w = cache->size.x * scale.x;
            rect.h = cache->size.y * scale.y;
        }

        SDL_RenderCopy(gf2d_graphics_get_renderer(),cache->texture,NULL, &rect);        
        return;
    }

    font = gfc_list_get_nth(font_manager.fonts,style);
    if(!font)
    {
        slog("Error retrieving font style: %i, for text: %s",style,text);
        return;
    }

    surface = TTF_RenderUTF8_Blended_Wrapped(font,text,gfc_color_to_sdl(color),0);
    if(!surface)
    {
        slog("surface from TTF_Render fail");
        return;
    }

    surface = gf2d_graphics_screen_convert(&surface);
    if(!surface)
    {
        slog("failed to convert screen");
        return;
    }

    texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),surface);
    if(!texture)
    {
        SDL_FreeSurface(surface);
        slog("failed to create texture");
        return;
    }

    
    rect.x = position.x;
    rect.y = position.y;

    if((!scale.x) || (!scale.y))
    {
        rect.w = surface->w;
        rect.h = surface->h;
    }
    else
    {
        rect.w = surface->w * scale.x;
        rect.h = surface->h * scale.y;
    }

    SDL_RenderCopy(gf2d_graphics_get_renderer(),texture,NULL, &rect);
    font_add_texture(text,style,color,texture,vector2d(surface->w,surface->h));
    SDL_FreeSurface(surface);
}

//gets the text size of the text object
void text_get_size(Text *self)
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    FontCache *cache;

    if(!self) return;
    cache = font_get_texture(self->text,self->style,self->color);
    if(cache)
    {
        //slog("getting here");
        //texture = cache->texture;

        self->textSize.x = cache->size.x;
        self->textSize.y = cache->size.y;
  
        return;
    }

    font = gfc_list_get_nth(font_manager.fonts,self->style);
    if(!font)
    {
        slog("Error retrieving font style: %i, for text: %s",self->style,self->text);
        return;
    }

    surface = TTF_RenderUTF8_Blended_Wrapped(font,self->text,gfc_color_to_sdl(self->color),0);
    if(!surface)
    {
        slog("surface from TTF_Render fail");
        return;
    }

    surface = gf2d_graphics_screen_convert(&surface);
    if(!surface)
    {
        slog("failed to convert screen");
        return;
    }

    texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),surface);
    if(!texture)
    {
        SDL_FreeSurface(surface);
        slog("failed to create texture");
        return;
    }

    self->textSize.x = surface->w;
    self->textSize.y = surface->h;
    
    font_add_texture(self->text,self->style,self->color,texture,vector2d(surface->w,surface->h));
    SDL_FreeSurface(surface);
}

Text *text_new(char *text, FontStyles style, Vector2D position, Vector2D scale, Color color, Uint32 ttl, TextType type)
{
    int i;
    Bool found = false;
    Text *newText;

    if(!text)
    {
        slog("No text included. Why have a text object with no text dummy?");
        return NULL;
    }
    //slog("new ent called");

    for(i = 0; i < text_manager.text_max; i++)
    {
        if(text_manager.text_list[i]._inuse)
        continue;

        memset(&text_manager.text_list[i],0,sizeof(Text));
        text_manager.text_list[i]._inuse = 1;
        newText = &text_manager.text_list[i];
        found = true;
        break;

    }

    if(!found)
    {
        slog("No more available text objects");
        return NULL;
    }

    newText->text = text;
    newText->style = style;
    newText->position = position;
    newText->scale = scale;
    newText->type = type;

    if(!ttl)
        newText->ttl = 0;
    else
        newText->ttl = SDL_GetTicks() + ttl;

    newText->color = color;


    switch(type)
    {
        case TT_MainMenu_Button:
        newText->moused = text_main_menu_button_moused;
        newText->update = text_main_menu_button_update;
        if(!strcmp(newText->text, "Play"))
        newText->leftClick = text_play_left_click;
        if(!strcmp(newText->text, "PvP"))
        newText->leftClick = text_pvp_left_click;
        break;
        default:
        slog("default type");
    }

    text_get_size(newText);

    newText->bounds = gfc_rect(position.x, position.y, newText->textSize.x * newText->scale.x, newText->textSize.y * newText->scale.y);

    return newText;
}

void text_free(Text *self)
{
    if(!self) return;

    self->_inuse = false;
}

void text_clear_all(Text *ignore)
{
    int i;
    for(i = 0; i < text_manager.text_max; i++)
    {
        //skip inactive
        if(!text_manager.text_list[i]._inuse)
        continue;

        //skip ignore
        if(&text_manager.text_list[i] == ignore)
        continue;

        //then free
        text_free(&text_manager.text_list[i]);
    }
}

void text_clear_perms(Text *ignore)
{
    int i;
    for(i = 0; i < text_manager.text_max; i++)
    {
        //skip inactive
        if(!text_manager.text_list[i]._inuse)
        continue;

        //skip ignore
        if(&text_manager.text_list[i] == ignore)
        continue;

        //then free if permanent
        if(text_manager.text_list[i].ttl == 0)
        text_free(&text_manager.text_list[i]);
    }
}


void text_update(Text *self)
{
    int mx,my;
    Uint32 mButton;

    if(!self)
    return;
    //any stuff that all text should do goes here

    if(self->ttl != 0 && self->ttl < SDL_GetTicks())
    {
        text_free(self);
        return;
    }

    self->mouse = false;

    self->bounds = gfc_rect(self->position.x,self->position.y,self->scale.x * self->textSize.x,self->scale.y * self->textSize.y);

    mButton = SDL_GetMouseState(&mx,&my);
    
    if(gfc_point_in_rect(vector2d(mx,my),self->bounds))
    {
        self->mouse = true;

        //call function for mouseover
        if(self->moused)
        self->moused(self);
    }
    
    if(self->mouse && mButton == SDL_BUTTON_LEFT && !leftClicked)
    {
        leftClicked = true;
        if(self->leftClick)
        self->leftClick(self);
    }

    if(self->mouse && mButton == 4 && !rightClicked)
    {
        //slog("happening");
        rightClicked = true;
        if(self->rightClick)
        self->rightClick(self);
    }
    
    if(self->update)
    self->update(self);
}

void text_update_all()
{
    int i;

    for(i = 0; i < text_manager.text_max; i++)
    {
        if(!text_manager.text_list[i]._inuse)
        continue;

        text_update(&text_manager.text_list[i]);
    }
}

void text_draw(Text *self)
{
    if(!self) return;

    if(!self->text) return;

    font_draw_text(self->text, self->style, self->color, self->position, self->scale); 
}

void text_draw_all()
{
    int i;

    for(i = 0; i < text_manager.text_max; i++)
    {
        if(!text_manager.text_list[i]._inuse)
        continue;

        text_draw(&text_manager.text_list[i]);
    }
}

void text_play_left_click(Text *self)
{
    if(!self) return;

    state = Choice;

    text_clear_all(NULL);

    Mix_Music *password = Mix_LoadMUS("audio/password-infinity.mp3");
    Mix_FadeInMusic(password,-1,4000);
    return;
}

void text_pvp_left_click(Text *self)
{
    Entity *player2;
    if(!self) return;

    state = Multiplayer;

    player2 = player_new(vector2d(1590,740));
    player2->type = Player2;
    player2->flip = vector2d(0,0);

    text_clear_all(NULL);

    Mix_Music *password = Mix_LoadMUS("audio/password-infinity.mp3");
    Mix_FadeInMusic(password,-1,4000);

    player_multi_combat_start(entity_get_player(),player2);
    gfc_list_append(tempIcons,icon_new(vector2d(1950,920),EndTurn));
    
    return;
}

void text_main_menu_button_moused(Text *self)
{
    if(!self) return;

    Color purple = gfc_color8(175, 99, 188, 255);

    if(!gfc_color_cmp(self->color, purple))
    self->color = purple;

    //gf2d_draw_rect(self->bounds,GFC_COLOR_WHITE);
}

void text_main_menu_button_update(Text *self)
{
    if(!self) return;

    if(!self->mouse && !gfc_color_cmp(self->color, GFC_COLOR_WHITE))
    self->color = GFC_COLOR_WHITE;
}