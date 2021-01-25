#ifndef SPRITEGROUP_H
#define SPRITEGROUP_H

#include "sprite.h"
#include "rendercontext.h"
#include <vector>
#include <optional>

class SpriteGroup {
    public:
    SpriteGroup(RenderContext renderContext);

    Sprite *create(std::string filename, int x = 0, int y = 0, int z = 0);

    ~SpriteGroup();

    void destroy(Sprite *sprite);

    void render();

    const std::vector<Sprite *>& sprites();

    void dispose();

    bool is_disposed();

    private:
    std::vector<Sprite *> m_sprites;
    std::optional<std::vector<Sprite *>> m_spritesSorted;
    RenderContext m_renderContext;
    bool m_disposed;
};

#endif