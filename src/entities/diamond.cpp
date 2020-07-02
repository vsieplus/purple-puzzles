// diamond implementation

#include "level/level.hpp"

#include "entities/diamond.hpp"

const std::string Diamond::DIAMOND_SHAPE = "diamond";

Diamond::Diamond(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) : 
    Movable(screenX, screenY, gridX, gridY, DIAMOND_VELOCITY, parity, 
    entitySprite, DIAMOND_SHAPE, entityAnimations) {

}

void Diamond::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Diamond::update(Level * level, float delta) {
    if(merging) {
        // start merge animation once movement onto receptor is finished
        if(!moving && !vanished && !entityAnimator.isAnimating()) {
            level->playSound(MERGE_SOUND_ID);
            activateAnimation(DIAMOND_MERGE);
            vanished = true;
        }
    } else if(moveDir != DIR_NONE) {
        checkReceptor(level);
    }

    // normal movement
    Movable::update(level, delta);
}

void Diamond::render(SDL_Renderer* renderer) const {    
    Movable::render(renderer);
}