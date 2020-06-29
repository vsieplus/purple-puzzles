// boost class implementation

#include "level/level.hpp"
#include "entities/boost.hpp"

Boost::Boost(int screenX, int screenY, int gridX, int gridY, int parity,
    int power, int direction, std::shared_ptr<Sprite> entitySprite,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) :
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite, entityAnimations), 
    power(power), direction((Direction)direction) {}

void Boost::handleEvents(const Uint8 * keyStates, Level * level) {
    
}

void Boost::update(Level * level, float delta) {
    if(activated) {
        // do some disappearing animation once player starts moving off
        if(!entityAnimator.isAnimating() && !vanished &&
            level->getGridElement<Entity>(gridX, gridY).get() == nullptr) {
            if(power == 1) {
                activateAnimation(BOOST_VANISH1);
            } else {
                activateAnimation(BOOST_VANISH2);
            }
            vanished = true;

            switch(direction) {
                case DIR_LEFT:      angle = 180.0;
                                    break;
                case DIR_RIGHT:     angle = 0.0;
                                    break;
                case DIR_UP:        angle = 270.0;
                                    break;
                case DIR_DOWN:      angle = 90.0;
                                    break;
                default:            break;
            }
        }
        
        Entity::update(level, delta);
    }
}

void Boost::render(SDL_Renderer* renderer) const {
    if(activated && vanished && !entityAnimator.isAnimating()) return;

    Entity::render(renderer);
}


Direction Boost::getDirection() const {
    return direction;
}

int Boost::getPower() const {
    return power;
}

bool Boost::isActivated() const {
    return activated;
}

void Boost::setActivated(bool activated) {
    this->activated = activated;

    // if deactivating reset rotation
    if(!activated) {
        angle = 0.0;
    }
}

void Boost::setVanished(bool vanished) {
    this->vanished = vanished;
}