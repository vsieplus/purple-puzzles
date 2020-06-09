// Abstract Entity class

#include "entities/entity.hpp"
#include "level/map.hpp"

Entity::Entity(int sX, int sY, int gX, int gY, std::shared_ptr<Sprite> entitySprite) : 
    screenX(sX), screenY(sY), gridX(gX), gridY(gY), entitySprite(entitySprite),
    renderArea{sX, sY, entitySprite->getWidth(), entitySprite->getHeight()} {}

/**
 * @brief Checks collision for current entity with the specified destination
 * 
 * @return true if there is a collision (including with the boundary)
 */
bool Entity::checkCollision(const Map & map, int destGridX, int destGridY) {
    // Check if new position is out of bounds, treat as collision (w/wall)
    if(!map.inBounds(destGridX, destGridY)) return true;

    // check collision with dest position. Return true if non-null entity
    bool entityAtNewPos = map.getGridElement(destGridX, destGridY) != nullptr;

    return entityAtNewPos;
}

void Entity::render(SDL_Renderer * renderer) const {
    entitySprite->render(renderer, renderArea);
}

// return the coordinates for the specified direction, relative to the entity
std::pair<int, int> Entity::getCoords(Direction direction) {
    switch(direction) {
        case DIR_UP:
            return std::pair<int, int> (gridX, gridY - 1);
            break;
        case DIR_DOWN:
            return std::pair<int, int> (gridX, gridY + 1);
            break;
        case DIR_LEFT:
            return std::pair<int, int> (gridX - 1, gridY);
            break;
        case DIR_RIGHT:
            return std::pair<int, int> (gridX + 1, gridY);
            break;
        case DIR_NONE:
            return std::pair<int, int> (gridX, gridY);
            break;
        default:
            return std::pair<int, int> (gridX, gridY);
            break;
    }
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