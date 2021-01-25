#include "spritegroup.h"
#include <algorithm>
#include <iostream>

SpriteGroup::SpriteGroup(RenderContext renderContext) {
    m_renderContext = renderContext;
}

Sprite *SpriteGroup::create(std::string filename, int x, int y, int z) {
    Sprite *sprite = new Sprite([&]() {
        m_spritesSorted.reset();
    }, m_renderContext, filename, x, y, z);
    m_sprites.push_back(sprite);
    return sprite;
}

SpriteGroup::~SpriteGroup() {
    dispose();
}

void SpriteGroup::destroy(Sprite *sprite) {
    for (auto it = m_sprites.begin(); it != m_sprites.end(); ++it) {
        int index = it - m_sprites.begin();
        if (m_sprites[index] == sprite) {
            m_sprites.erase(it);
            break;
        }
    }
    delete sprite;
}

void SpriteGroup::render() {
    // std::vector<Sprite *> sprites = m_sprites;
    if (!m_spritesSorted.has_value()) {
        m_spritesSorted = m_sprites;
        std::vector<Sprite *> &sprites = m_spritesSorted.value();
        std::cout << "re-sorting sprites" << std::endl;
        std::sort(sprites.begin(), sprites.end(), [&](Sprite *s1, Sprite *s2) {
            return s1->get_z() < s2->get_z();
        });
    }
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    for (Sprite *sprite : m_spritesSorted.value()) {
        sprite->render(DM.w, DM.h);
    }
}

const std::vector<Sprite *>& SpriteGroup::sprites() { return m_sprites; }

void SpriteGroup::dispose() {
    if (m_disposed)
        return;
    
    for (auto it = m_sprites.begin(); it != m_sprites.end(); ++it) {
        delete m_sprites[it - m_sprites.begin()];
    }

    m_disposed = true;
}

bool SpriteGroup::is_disposed() {
    return m_disposed;
}