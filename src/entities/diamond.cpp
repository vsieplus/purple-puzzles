#include "entities/diamond.hpp"

Diamond::Diamond(int screenX, int screenY, int gridX, int gridY,
    std::shared_ptr<Sprite> entitySprite, int parity) : Movable(screenX,
    screenY, gridX, gridY, DIAMOND_VELOCITY, entitySprite, parity) {

}

void Diamond::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Diamond::update(Level * level, float delta) {
    Movable::update(level, delta);

    // check for receptor
}

void Diamond::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
}