// Implementation for level class

#include "memswap.hpp"
#include "level/level.hpp"
#include "entities/player.hpp"

Level::Level() {}

Level::Level(std::string tiledMapPath, SDL_Renderer * renderer) : map(tiledMapPath) {
    map.loadMap(renderer, this);

    // initialize entity grid
    initGrid();
    grid[0] = std::make_shared<Player>(0, 0, 0, 0, renderer);
}

Level::~Level(){
    grid.clear();
}

void Level::initGrid() {
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int currIdx = xyToIndex(x,y);
            grid[currIdx] = nullptr;
        }
    }
}

// Event loop
void Level::handleEvents(const Uint8 * keyStates) {
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int currIdx = xyToIndex(x,y);
            if(grid.at(currIdx)) {
                grid.at(currIdx)->handleEvents(keyStates, this);
            }
        }
    }
}

// Update loop
void Level::update() {
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int currIdx = xyToIndex(x,y);
            if(grid.at(currIdx)) {
                grid.at(currIdx)->update(this);
            }
        }
    }
}

void Level::render(SDL_Renderer * renderer) const {
    // Render the map
    map.render(renderer); 

    // Render the objs in the grid
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int currIdx = xyToIndex(x,y);
            if(grid.at(currIdx)) {
                grid.at(currIdx)->render(renderer);
            }
        }
    }    
}

// Add entities from map to entity grid
void Level::addEntityTiles(const tmx::TileLayer * tileLayer, 
    const std::map<int, std::shared_ptr<SDL_Texture>> & tilesetTextures) {

}

// Update size from the given map
void Level::updateSize(const tmx::Map & map, int tileWidth, int tileHeight) {
    auto map_dimensions = map.getTileCount();

    gridWidth = map_dimensions.x;
    gridHeight = map_dimensions.y;

    pixelWidth = tileWidth * gridWidth;
    pixelHeight = tileHeight * gridHeight;
}

int Level::xyToIndex(int x, int y) const {
    return x + y * gridWidth;
}

std::pair<int, int> Level::indexToXY(int index) const {
    int x = index % gridWidth;
    int y = index / gridWidth;
    return std::make_pair(x,y);
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

const std::unordered_map<int, std::shared_ptr<Entity>> & Level::getGrid() {
    return grid;
}

// Move a grid element from start x,y to end
void Level::setGridElement(int startX, int startY, int endX, int endY) {
    grid[xyToIndex(endX, endY)] = std::move(grid[xyToIndex(startX, startY)]);
}