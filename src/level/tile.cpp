// Implementation for tile class

#include "memswap.hpp"

#include "level/level.hpp"
#include "level/tile.hpp"

Tile::Tile(int mapX, int mapY, int tileParity, std::shared_ptr<Sprite> tileSprite,
    MemSwap * game) 
    : Entity(mapX, mapY, tileParity, tileSprite, {
        {TILE_FLIP, game->getResManager().getAnimation(TILE_FLIP_ID)}
    }) {
    
    purpleStatus.fill(false);

    if(parity == PARITY_PURPLE) {
        flipped = true;
        purpleStatus[DIR_NONE] = true;
    }
}

void Tile::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Tile::update(Level * level, float delta) {
    Entity::update(level, delta);
}

void Tile::render(SDL_Renderer * renderer) const {
    Entity::render(renderer);

    // render tile borders
}

// Filp tile's parity, + update sprite
void Tile::flip(std::shared_ptr<Sprite> newTileSprite) {
    parity = parity == PARITY_GRAY ? PARITY_PURPLE : PARITY_GRAY;
    entitySprite.reset();
    entitySprite = newTileSprite;
    flipped = true;

    // flipping animation
    activateAnimation(TILE_FLIP);
}

bool Tile::isFlipped() const {
    return flipped;
}