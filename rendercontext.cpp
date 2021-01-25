#include "rendercontext.h"

RenderContext::RenderContext(SDL_Window *w, SDL_Renderer *r) : window{w}, renderer{r} {

}
RenderContext::RenderContext() : window {nullptr}, renderer {nullptr} {

}