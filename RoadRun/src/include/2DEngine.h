#pragma once

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "Physics.h"
#include "gameClock.h"
#include "Mixer.h"

using namespace std;

class Engine2D
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Engine2D(const char *name, int w, int h, Uint32 flags);

    int window_w, window_h;
    int resolution_w, resolution_h;

    const Uint8 *key;
    int cursor_x, cursor_y;
    int rcursor_x, rcursor_y;
    bool mousebutton_r, mousebutton_l;
    int scroll_wheel_x, scroll_wheel_y;

    char text[32];
    int textlen = 0;
    SDL_Keycode keyboard_sym;

    void cursor(int en);

    void color(Uint8 r, Uint8 g, Uint8 b);

    void draw_pixel(int x, int y);
    void draw_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);
    void draw_line(int x1, int y1, int x2, int y2);
    void draw_line(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

    void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b);

    void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b);

    void draw_texture(SDL_Texture *tex, int x1, int y1, int x2, int y2);
    void draw_texture(SDL_Texture *tex, int x, int y);
    void draw_texture(SDL_Texture *tex, int x, int y, double size);
    void draw_texture_wh(SDL_Texture *tex, int x, int y, int w, int h);

    // void draw_text(string text, int size, bool outline, int x, int y);

    void fill(int x, int y, int w, int h);
    bool update();

    SDL_Texture *load_texture(const char *path);

private:
    SDL_Color rgb;

    char composition[32];
    Sint32 textcursor;
    Sint32 selection_len;

    TTF_Font *font32;
    TTF_Font *font32_outline;
    TTF_Font *font24;
    TTF_Font *font24_outline;
    TTF_Font *font16;
    TTF_Font *font16_outline;
};

Engine2D::Engine2D(const char *name, int w, int h, Uint32 flags)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    window_w = w;
    window_h = h;
    resolution_w = w;
    resolution_h = h;

    const char *font_path = "res/font/cocogoose.ttf";

    font32 = TTF_OpenFont(font_path, 32);
    font32_outline = TTF_OpenFont(font_path, 32);
    font24 = TTF_OpenFont(font_path, 24);
    font24_outline = TTF_OpenFont(font_path, 24);
    font16 = TTF_OpenFont(font_path, 16);
    font16_outline = TTF_OpenFont(font_path, 16);

    if (font32_outline)
        TTF_SetFontOutline(font32_outline, 3);
    if (font24_outline)
        TTF_SetFontOutline(font24_outline, 3);
    if (font16_outline)
        TTF_SetFontOutline(font16_outline, 3);

    SDL_StartTextInput();

    update();
}

bool Engine2D::update()
{
    SDL_RenderPresent(renderer);
    SDL_GetWindowSize(window, &window_w, &window_h);
    SDL_GetMouseState(&cursor_x, &cursor_y);
    SDL_GetRelativeMouseState(&rcursor_x, &rcursor_y);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mousebutton_l = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                mousebutton_r = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mousebutton_l = false;
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                mousebutton_r = false;
            }
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) // scroll up
            {
                scroll_wheel_y -= 1;
            }
            else if (event.wheel.y < 0) // scroll down
            {
                scroll_wheel_y += 1;
            }

            if (event.wheel.x > 0) // scroll right
            {
                scroll_wheel_x += 1;
            }
            else if (event.wheel.x < 0) // scroll left
            {
                scroll_wheel_x -= 1;
            }
            break;

        case SDL_TEXTINPUT:
            strcat(text, event.text.text);
            textlen += 1;
            break;

        case SDL_TEXTEDITING:
            strcpy(composition, event.edit.text);
            textcursor = event.edit.start;
            selection_len = event.edit.length;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && textlen > 1)
            {
                text[textlen - 1] = NULL;
                textlen--;
            }
            switch (event.key.keysym.sym)
            {
            default:
                keyboard_sym = event.key.keysym.sym;
            }
            break;

        default:
            break;
        }
    }

    key = SDL_GetKeyboardState(NULL);

    return true;
}

void Engine2D::cursor(int en)
{
    SDL_ShowCursor(en);
}

void Engine2D::color(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
}

void Engine2D::draw_pixel(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void Engine2D::draw_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_pixel(x, y);
}

void Engine2D::draw_line(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Engine2D::draw_line(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_line(x1, y1, x2, y2);
}

void Engine2D::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

void Engine2D::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    draw_triangle(x1, y1, x2, y2, x3, y3);
}

void Engine2D::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1 > y2)
    {
        swap(y1, y2);
        swap(x1, x2);
    }
    if (y1 > y3)
    {
        swap(y1, y3);
        swap(x1, x3);
    }
    if (y2 > y3)
    {
        swap(y2, y3);
        swap(x2, x3);
    }

    t1x = t2x = x1;
    y = y1; // Starting points
    dx1 = (int)(x2 - x1);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1);
    if (dx2 < 0)
    {
        dx2 = -dx2;
        signx2 = -1;
    }
    else
        signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1)
    { // swap values
        swap(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2)
    { // swap values
        swap(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2)
        goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            i++;
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                    t1xp = signx1; // t1x += signx1;
                else
                    goto next1;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2; // t2x += signx2;
                else
                    goto next2;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next2:
        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        draw_line(minx, y, maxx, y); // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2)
            break;
    }
next:
    // Second half
    dx1 = (int)(x3 - x2);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1)
    { // swap values
        swap(dy1, dx1);
        changed1 = true;
    }
    else
        changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                {
                    t1xp = signx1;
                    break;
                } // t1x += signx1;
                else
                    goto next3;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
            if (i < dx1)
                i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2;
                else
                    goto next4;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next4:

        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        draw_line(minx, y, maxx, y);
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3)
            return;
    }
}

void Engine2D::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b)
{
    color(r, g, b);
    fill_triangle(x1, y1, x2, y2, x3, y3);
}

void Engine2D::draw_texture(SDL_Texture *tex, int x1, int y1, int x2, int y2)
{
    draw_texture_wh(tex, x1, y1, x2 - x1, y2 - y1);
}

void Engine2D::draw_texture(SDL_Texture *tex, int x, int y)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 0;
    src.h = 0;
    SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = mapper(x, 0, resolution_w, 0, window_w);
    dst.y = mapper(y, 0, resolution_h, 0, window_h);
    dst.w = mapper(src.w, 0, resolution_w, 0, window_w);
    dst.h = mapper(src.h, 0, resolution_w, 0, window_w);

    SDL_RenderCopy(renderer, tex, &src, &dst);
}

void Engine2D::draw_texture(SDL_Texture *tex, int x, int y, double size)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 0;
    src.h = 0;
    SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = mapper(x, 0, resolution_w, 0, window_w);
    dst.y = mapper(y, 0, resolution_h, 0, window_h);
    dst.w = mapper(src.w * size, 0, resolution_w, 0, window_w);
    dst.h = mapper(src.h * size, 0, resolution_w, 0, window_w);

    SDL_RenderCopy(renderer, tex, &src, &dst);
}

void Engine2D::draw_texture_wh(SDL_Texture *tex, int x, int y, int w, int h)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 0;
    src.h = 0;
    SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = mapper(x, 0, resolution_w, 0, window_w);
    dst.y = mapper(y, 0, resolution_h, 0, window_h);
    dst.w = mapper(w, 0, resolution_w, 0, window_w);
    dst.h = mapper(h, 0, resolution_h, 0, window_h);

    SDL_RenderCopy(renderer, tex, &src, &dst);
}
/*
void Engine2D::draw_text(string text, int size, bool outline, int x, int y)
{
    TTF_Font *font;
    switch (size)
    {
    case 16:
        if (outline)
        {
            font = font16_outline;
        }
        else
        {
            font = font16;
        }
        break;

    case 24:
        if (outline)
        {
            font = font24_outline;
        }
        else
        {
            font = font24;
        }
        break;

    case 32:
        if (outline)
        {
            font = font32_outline;
        }
        else
        {
            font = font32;
        }
        break;

    default:
        font = font24;
        break;
    }

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), rgb);
    SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h;

    SDL_Rect dst;
    dst.x = mapper(x, 0, resolution_w, 0, window_w);
    dst.y = mapper(y, 0, resolution_h, 0, window_h);
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dst);
    SDL_FreeSurface(surfaceMessage);
}*/

void Engine2D::fill(int x, int y, int w, int h)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_RenderFillRect(renderer, &r);
}

SDL_Texture *Engine2D::load_texture(const char *path)
{
    return IMG_LoadTexture(renderer, path);
}