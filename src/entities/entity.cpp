// Abstract Entity class

#include "entities/entity.hpp"
#include "level/level.hpp"
#include "level/map.hpp"

Entity::Entity(int sX, int sY, int gX, int gY, std::string texturePath, 
    SDL_Renderer * renderer) : screenX(sX), screenY(sY), gridX(gX), gridY(gY) {
    texture.loadTexture(texturePath, renderer);
}

/**
 * @brief Checks collision for current entity with the specified destination
 * 
 * @return true if there is a collision (including with the boundary)
 */
bool Entity::checkCollision(Level * level, int destGridX, int destGridY) {
    // Check if new position is out of bounds, treat as collision (w/wall)
    if(!level->getMap().inBounds(destGridX, destGridY)) return true;

    // check collision with dest position. Return true if non-null entity
    bool entityAtNewPos = 
        level->getGrid().at(level->xyToIndex(destGridX, destGridY)) != nullptr;

    return entityAtNewPos;
}

int Entity::getScreenX() const {
    return screenX;
}

int Entity::getScreenY() const {
    return screenY;
}

int Entity::getGridX() const {
    return gridX;
}

int Entity::getGridY() const {
    return gridY;
}