#include "sprite.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <functional>

std::unordered_map<std::string, std::pair<SDL_Texture *, int>> Sprite::texCache;

Sprite::Sprite(const std::function<void()> &invalidateSort, RenderContext rc, std::string filename, int X, int Y, int Z)
        : m_x{X}, m_y{Y}, m_z{Z}, m_oldx{X}, m_oldy{Y}, m_oldz{Z} {
    
    m_renderContext = rc;
    m_filename = filename;
    fInvalidateSort = invalidateSort;

    if (texCache.find(filename) == texCache.end()) {
        load_texture();
    } else {
        m_texture = texCache[filename].first;
        texCache[filename].second += 1;
    }

    Uint32 format;
    int access;
    if (SDL_QueryTexture(m_texture, &format, &access, &m_width, &m_height)) {
        throw SpriteError("Failed to SDL_QueryTexture");
    }
    fInvalidateSort();
}
Sprite::~Sprite() {
    texCache[m_filename].second -= 1;
    if (texCache[m_filename].second == 0) {
        std::cout << "All references to texture gone, deallocating" << std::endl;
        SDL_DestroyTexture(m_texture);
        texCache.erase(m_filename);
    }
}

int Sprite::get_x() {
    return m_x;
}
int Sprite::get_y() {
    return m_y;
}
int Sprite::get_z() {
    return m_z;
}

void Sprite::set_x(int v) {
    m_x = v;
    fInvalidateSort();
}
void Sprite::set_y(int v) {
    m_y = v;
    fInvalidateSort();
}
void Sprite::set_z(int v) {
    m_z = v;
    fInvalidateSort();
}

void Sprite::render(int screenWidth, int screenHeight) {
    SDL_Rect rect = {m_x, m_y, m_width, m_height};
    SDL_RenderCopy(m_renderContext.renderer, m_texture, NULL, &rect);
}

int Sprite::get_width()  { return m_width;  }
int Sprite::get_height() { return m_height; }

void Sprite::load_texture() {
    SDL_Surface *img, *convertedImg;
    SDL_PixelFormat *pixelFormat;

    img = IMG_Load(m_filename.c_str());
    if (img == nullptr) {
        throw SpriteError(SDL_GetError());
    }

    pixelFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(m_renderContext.window));
    convertedImg = SDL_ConvertSurface(img, pixelFormat, 0);
    SDL_FreeFormat(pixelFormat);
    if (convertedImg == nullptr) {
        throw SpriteError("Failed to convert image pixel format to window pixel format");
    }
    SDL_FreeSurface(img);

    m_texture = SDL_CreateTextureFromSurface(m_renderContext.renderer, convertedImg);
    if (m_texture == nullptr) {
        throw SpriteError("Failed to convert surface to texture");
    }
    SDL_FreeSurface(convertedImg);

    texCache[m_filename] = std::make_pair(m_texture, 1);
}

SpriteError::SpriteError(std::string msg) {
    m_msg = msg;
}
const char* SpriteError::what() const noexcept {
    return m_msg.c_str();
}