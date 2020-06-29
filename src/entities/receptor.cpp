// receptor implementation

#include "entities/receptor.hpp"

Receptor::Receptor(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite, std::string shape,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) : 
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite, entityAnimations), 
    shape(shape) {}


void Receptor::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Receptor::update(Level * level, float delta) {
    // check if completed
    if(completed) {
        // check if merge animation set/complete
        Entity::update(level, delta);
    }
}

void Receptor::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
}

void Receptor::setCompleted(bool completed) {
    this->completed = completed;
}

bool Receptor::isCompleted() const {
    return completed;
}

std::string Receptor::getShape() const {
    return shape;
}