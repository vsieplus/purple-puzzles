// diamond implementation

#include "level/level.hpp"

#include "entities/diamond.hpp"
#include "entities/receptor.hpp"

Diamond::Diamond(int screenX, int screenY, int gridX, int gridY,
    std::shared_ptr<Sprite> entitySprite, int parity) : Movable(screenX,
    screenY, gridX, gridY, DIAMOND_VELOCITY, entitySprite, parity) {

}

void Diamond::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Diamond::update(Level * level, float delta) {
    // if merging, update receptor, and then do 'merge' animation
    if(merging) {
        dReceptor->update(level, delta);
    }

    // check for receptor
    if(moveDir != DIR_NONE) {
        checkReceptor(level);
    }
    
    // normal movement
    Movable::update(level, delta);
}

void Diamond::render(SDL_Renderer* renderer) const {
    // render receptor first when merging
    if(merging) {
        dReceptor->render(renderer);
    }
    
    Movable::render(renderer);
}

void Diamond::checkReceptor(Level * level) {
    std::pair<int, int> pushCoords = getCoords(moveDir);

    // check for receptor at tile we're going to check
    auto receptor = level->getMap().getGridElement<Receptor>(pushCoords.first,
        pushCoords.second);

    if(receptor.get() && !receptor->isCompleted()) {
        merging = true;
        receptor->setCompleted(true);

        // remove receptor from grid and track receptor from this diamond
        dReceptor = receptor;
        level->getMap().removeGridElement(receptor->getGridX(), receptor->getGridY());
    }
}