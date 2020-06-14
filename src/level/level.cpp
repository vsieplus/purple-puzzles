// Implementation for level class

#include "memswap.hpp"
#include "level/level.hpp"
#include "entities/player.hpp"

Level::Level() {}

Level::Level(std::string tiledMapPath, SDL_Renderer * renderer, MemSwap * game) 
    : map(tiledMapPath, renderer, this, game) {}

// Event loop (down right)
void Level::handleEvents(const Uint8 * keyStates) {
    map.handleEvents(this, keyStates);
}

// Update loop
void Level::update(float delta) {
    // Update the map
    map.update(this, delta);
}

void Level::render(SDL_Renderer * renderer) const {
    // Render the map
    map.render(renderer); 
}

// flip tiles in the map for the specified movement
void Level::flipMapTiles(int movedFromX, int movedFromY, int entityParity) {
    map.flipTile(movedFromX, movedFromY, entityParity, this);
}

// Update size from the given map
void Level::updateSize(const tmx::Map & map, int tileWidth, int tileHeight) {
    auto map_dimensions = map.getTileCount();

    gridWidth = map_dimensions.x;
    gridHeight = map_dimensions.y;

    pixelWidth = tileWidth * gridWidth;
    pixelHeight = tileHeight * gridHeight;
}

int Level::getGridWidth() const { 
    return gridWidth;
}

int Level::getGridHeight() const { 
    return gridHeight;
}

int Level::getPixelWidth() const { 
    return pixelWidth;
}

int Level::getPixelHeight() const { 
    return pixelHeight;
}

bool Level::inBounds(int x, int y) const {
    return map.inBounds(x,y);
}

Parity Level::getTileParity(int x, int y) const {
    return map.getTileParity(x, y);
}

// Move a grid element from start x,y to end
void Level::moveGridElement(int startX, int startY, int endX, int endY) {
    map.moveGridElement(startX, startY, endX, endY);
}

void Level::removeGridElement(int x, int y) {
    map.removeGridElement(x, y);
}