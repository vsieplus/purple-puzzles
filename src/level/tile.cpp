// Implementation for tile class

#include "memswap.hpp"

#include "level/level.hpp"
#include "level/tile.hpp"

Tile::Tile(int mapX, int mapY, int tileParity, std::shared_ptr<Sprite> tileSprite,
    MemSwap * game) 
    : Entity(mapX, mapY, tileParity, tileSprite, {
        {TILE_FLIP, game->getResManager().getAnimation(TILE_FLIP_ID)}
    }) {}

void Tile::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Tile::update(Level * level, float delta) {
    Entity::update(level, delta);
}

void Tile::render(SDL_Renderer * renderer) const {
    Entity::render(renderer);
}

// Filp tile's parity, + update sprite
void Tile::flip(std::shared_ptr<Sprite> newTileSprite, bool undo) {
    parity = parity == PARITY_GRAY ? PARITY_PURPLE : PARITY_GRAY;
    entitySprite.reset();
    entitySprite = newTileSprite;
    flipped = true;

    // flipping animation if not undo
    if(!undo) {
        activateAnimation(TILE_FLIP);
    }
}

bool Tile::isFlipped() const {
    return flipped;
}