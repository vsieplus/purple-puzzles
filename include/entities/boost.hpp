// Boost entity class

#ifndef BOOST_HPP
#define BOOST_HPP

#include "entities/entity.hpp"

class Boost : public Entity {
    private:

        // how far the boost sends an entity
        int power;

        // the direction the boost is facing
        Direction direction;

        bool activated = false;

    public:
        Boost(int screenX, int screenY, int gridX, int gridY, 
            std::shared_ptr<Sprite> entitySprite, int power, int direction);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;


        Direction getDirection() const;
        int getPower() const;
        bool isActivated() const;
        void setActivated(bool activated);

};

#endif // BOOST_HPP