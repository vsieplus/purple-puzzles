// movable diamond object

#ifndef DIAMOND_HPP
#define DIAMOND_HPP

#include "entities/movable.hpp"

class Diamond : public Movable {
    private:
        static const int DIAMOND_VELOCITY = 5;

    public:
        Diamond(int screenX, int screenY, int gridX, int gridY,
            std::shared_ptr<Sprite> entitySprite, int parity);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;
};

#endif // DIAMOND_HPP