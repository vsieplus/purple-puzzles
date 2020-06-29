// Boost entity class

#ifndef BOOST_HPP
#define BOOST_HPP

#include "entities/entity.hpp"

class Boost : public Entity {
    private:
        enum BoostAnimation {BOOST_VANISH1, BOOST_VANISH2};

        // how far the boost sends an entity
        int power;

        // the direction the boost is facing
        Direction direction;

        bool activated = false;

    public:
        Boost(int screenX, int screenY, int gridX, int gridY, int parity,
            int power, int direction, std::shared_ptr<Sprite> entitySprite,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;


        Direction getDirection() const;
        int getPower() const;
        bool isActivated() const;
        void setActivated(bool activated);

};

#endif // BOOST_HPP