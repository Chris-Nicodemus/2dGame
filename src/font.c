#include "simple_logger.h"
#include "gfc_list.h"
#include "gf2d_graphics.h"
#include "font.h"

typedef struct
{
    List *fonts;
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

void font_draw_text(const char* text, FontStyles style, Color color, Vector2D position, Vector2D scale)
{
    TTF_Font *font = gfc_list_get_nth(font_manager.fonts,style);
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;

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
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}