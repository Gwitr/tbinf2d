#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "rendercontext.h"
#include <unordered_map>
#include <functional>

class Sprite {
    public:

    Sprite(const std::function<void()>&, RenderContext, std::string, int, int, int);
    ~Sprite();

    void render(int, int);

    int get_width();
    int get_height();

    int get_x();
    int get_y();
    int get_z();

    void set_x(int);
    void set_y(int);
    void set_z(int);

    static std::unordered_map<std::string, std::pair<SDL_Texture *, int>> texCache;

    private:
    std::string m_filename;
    SDL_Texture *m_texture;
    RenderContext m_renderContext;
    int m_width, m_height;
    int m_x, m_y, m_z;
    int m_oldx, m_oldy, m_oldz;
    std::function<void()> fInvalidateSort;

    void load_texture();
};

class SpriteError : public std::exception {
    public:
    SpriteError(std::string msg);
    const char* what() const noexcept override;

    private:
    std::string m_msg;
};

#endif