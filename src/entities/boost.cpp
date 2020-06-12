// boost class implementation

#include "entities/boost.hpp"

Boost::Boost(int screenX, int screenY, int gridX, int gridY, 
    std::shared_ptr<Sprite> entitySprite, int power, int direction) :
    Entity(screenX, screenY, gridX, gridY, entitySprite), power(power),
    direction((Direction)direction) {}

void Boost::handleEvents(const Uint8 * keyStates, Level * level) {
    
}

void Boost::update(Level * level, float delta) {
    if(activated) {
        // do some disappearing animation
    }
}

void Boost::render(SDL_Renderer* renderer) const {
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
}