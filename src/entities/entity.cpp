// Abstract Entity class

#include "entities/entity.hpp"
#include "level/map.hpp"
#include "level/level.hpp"

Entity::Entity(int screenX, int screenY, int gridX, int gridY, int parity, 
    std::shared_ptr<Sprite> entitySprite) : 
    gridX(gridX), gridY(gridY), parity((Parity)parity), entitySprite(entitySprite),
    renderArea{screenX, screenY, entitySprite->getWidth(), 
    entitySprite->getHeight()} {}

Entity::Entity(int screenX, int screenY, int parity, 
    std::shared_ptr<Sprite> entitySprite) : gridX(0), gridY(0), parity(Parity(parity)),
    entitySprite(entitySprite), renderArea{screenX, screenY, entitySprite->getWidth(), 
    entitySprite->getHeight()} {}

/**
 * @brief Checks collision for current entity with the specified destination
 * 
 * @return true if there is a collision (including with the boundary)
 */
bool Entity::checkCollision(Level * level, int destGridX, int destGridY) {
    // Check if new position is out of bounds, treat as collision (w/wall)
    if(!level->inBounds(destGridX, destGridY)) return true;

    // check collision with dest position. Return true if non-null entity
    bool entityAtNewPos = 
        level->getGridElement<Entity>(destGridX, destGridY) != nullptr;

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

void Entity::setScreenX(int x) {
    renderArea.x = x;
}

void Entity::setScreenY(int y) {
    renderArea.y = y;
}

int Entity::getScreenX() const {
    return renderArea.x;
}

int Entity::getScreenY() const {
    return renderArea.y;
}

void Entity::setGridX(int x) {
    gridX = x;
}

void Entity::setGridY(int y) {
    gridY = y;
}

int Entity::getGridX() const {
    return gridX;
}

int Entity::getGridY() const {
    return gridY;
}

Parity Entity::getParity() const {
    return parity;
}