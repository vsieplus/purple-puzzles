#ifndef TILE_HPP
#define TILE_HPP

#include <array>
#include <SDL.h>

#include "utils/sprite.hpp"

#include "entities/entity.hpp"

class Level;

class Tile : public Entity {
    private:
        bool flipped = false;

        // tile animations
        enum TileAnimation {TILE_FLIP};
        
    public:
        Tile(int gridX, int gridY, int tileParity, std::shared_ptr<Sprite> sprite,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;

        void render(SDL_Renderer * renderer) const;

        // flip the tile parity
        void flip(std::shared_ptr<Sprite> newTileSprite, bool undo);
        
        bool isFlipped() const;
};

#endif