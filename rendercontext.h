#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <SDL2/SDL.h>

class RenderContext {
    public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    RenderContext(SDL_Window *w, SDL_Renderer *r);
    RenderContext();
};

#endif