#include "simple_logger.h"
#include "gfc_list.h"
#include "gf2d_graphics.h"
#include "font.h"

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

static FontManager font_manager = {0};
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
    TTF_Quit();
}

void font_init()
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

        atexit(font_close);
    }
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