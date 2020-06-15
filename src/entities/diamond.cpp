// diamond implementation

#include "level/level.hpp"

#include "entities/diamond.hpp"

const std::string Diamond::DIAMOND_SHAPE = "diamond";

Diamond::Diamond(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite) : Movable(screenX, screenY, gridX, 
    gridY, DIAMOND_VELOCITY, parity, entitySprite, DIAMOND_SHAPE) {

}

void Diamond::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Diamond::update(Level * level, float delta) {
    if(moveDir != DIR_NONE) {
        checkReceptor(level);
    }

    // normal movement
    Movable::update(level, delta);
}

void Diamond::render(SDL_Renderer* renderer) const {    
    Movable::render(renderer);
}