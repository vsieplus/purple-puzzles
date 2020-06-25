// Implementation for level class

#include "memswap.hpp"
#include "level/level.hpp"
#include "entities/portal.hpp"

Level::Level() {}

Level::Level(std::string tiledMapPath, SDL_Renderer * renderer, MemSwap * game) 
    : map(tiledMapPath, renderer, this, game), mapPath(tiledMapPath) {}

// Event loop (down right)
void Level::handleEvents(const Uint8 * keyStates) {
    map.handleEvents(this, keyStates);
}

// Update loop
void Level::update(float delta) {
    if(!completed) {
        // Update the map if not completed
        map.update(this, delta);
    }
}

void Level::render(SDL_Renderer * renderer) const {
    // Render the map
    map.render(renderer); 
}

// Update size from the given map
void Level::updateSize(const tmx::Map & map, int tileWidth, int tileHeight) {
    auto map_dimensions = map.getTileCount();

    gridWidth = map_dimensions.x;
    gridHeight = map_dimensions.y;

    pixelWidth = tileWidth * gridWidth;
    pixelHeight = tileHeight * gridHeight;
}

// flip tiles in the map for the specified movement
void Level::flipMapTiles(int movedFromX, int movedFromY, int entityParity, bool undo) {
    map.flipTile(movedFromX, movedFromY, entityParity, this, undo);
}

// Move a grid element from start x,y to end
void Level::moveGridElement(int startX, int startY, int endX, int endY) {
    map.moveGridElement(startX, startY, endX, endY);
}

void Level::placeGridElement(std::shared_ptr<Entity> entity, int x, int y) {
    map.placeGridElement(entity, x, y);
}

void Level::removeGridElement(int x, int y) {
    map.removeGridElement(x, y);
}

// place portals back in grid
void Level::placePortals() {
    map.placePortals();
}

// check if the level is complete
void Level::checkComplete() {
    // check if each tile parity matches purple parity
    for(int y = 0; y < gridHeight; y++) {
        for(int x = 0; x < gridWidth; x++) {
            // skip tiles with portals
            if(map.hasPortals() && map.getGridElement<Portal>(x, y).get()) {
                continue;
            }

            // if some tile isn't flipped, level has not been succesfully completed
            if(getTileParity(x, y) != PARITY_PURPLE) {
                return;
            }
        }
    }

    completed = true;
}

void Level::reset(MemSwap * game) {
    // clear and reload the map tiles/entities
    map.clear();
    map.loadMap(mapPath, game->getRenderer(), this, game);

    tilesFlipped = 0;
    perfect = false;
}

bool Level::isCompleted() const {
    return completed;
}

bool Level::isPerfect() const {
    return perfect;
}

void Level::setPerfect(bool perfect) {
    this->perfect = perfect;
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

void Level::addTileFlipped() {
    tilesFlipped++;
}

int Level::getTilesFlipped() const {
    return tilesFlipped;
}

void Level::setTilesFlipped(int tiles) {
    tilesFlipped = tiles;
}

int Level::getMovesUndone() const {
    return map.getMovesUndone();
}

bool Level::inBounds(int x, int y) const {
    return map.inBounds(x,y);
}

Parity Level::getTileParity(int x, int y) const {
    return map.getTileParity(x, y);
}

std::shared_ptr<Player> Level::getPlayer() const {
    return map.getPlayer();
}