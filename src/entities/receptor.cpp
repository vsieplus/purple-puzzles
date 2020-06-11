// receptor implementation

#include "entities/receptor.hpp"

Receptor::Receptor(int screenX, int screenY, int gridX, int gridY, 
    std::shared_ptr<Sprite> entitySprite) : Entity(screenX, screenY, gridX,
    gridY, entitySprite) {}


void Receptor::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Receptor::update(Level * level, float delta) {
    // check if completed
    if(completed) {
        // check if merge animation set/complete
        
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