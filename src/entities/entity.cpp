// Abstract Entity class

#include "entities/entity.hpp"

Entity::Entity(int sX, int sY, int gX, int gY, std::string texturePath) :
    screenX(sX), screenY(sY), gridX(gX), gridY(gY) {
    texture.loadTexture(texturePath);
}


int Entity::getScreenX() {
    return screenX;
}

int Entity::getScreenY() {
    return screenY;
}

int Entity::getGridX() {
    return gridX;
}

int Entity::getGridY() {
    return gridY;
}