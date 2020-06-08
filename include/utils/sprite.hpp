// class representing a sprite in a spritesheet

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>

#include "utils/texture.hpp"

class Sprite {
    private:
        // shared ptr to the spritesheet texture containing the sprite
        std::shared_ptr<Texture> spritesheet;

        SDL_Rect spriteClip;

    public:
        Sprite(std::shared_ptr<Texture> spritesheet, const SDL_Rect & clip);

        void render(SDL_Renderer * renderer, const SDL_Rect & renderArea) const;

        int getWidth() const;
        int getHeight() const;
};

#endif // SPRITE_HPP