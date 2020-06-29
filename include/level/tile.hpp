#ifndef TILE_HPP
#define TILE_HPP

#include <array>
#include <SDL.h>

#include "utils/sprite.hpp"

#include "entities/entity.hpp"

class MemSwap;
class Level;

class Tile : public Entity {
    private:
        bool flipped = false;

        // tile animations
        enum TileAnimation {TILE_FLIP};

        inline static const std::string TILE_FLIP_ID = "tileFlip";
        
    public:
        Tile(int gridX, int gridY, int tileParity, std::shared_ptr<Sprite> sprite,
            MemSwap * game);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;

        void render(SDL_Renderer * renderer) const;

        // flip the tile parity
        void flip(std::shared_ptr<Sprite> newTileSprite, bool undo);
        
        bool isFlipped() const;
};

#endif