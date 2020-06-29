// movable diamond object

#ifndef DIAMOND_HPP
#define DIAMOND_HPP

#include "entities/movable.hpp"
#include "entities/receptor.hpp"

class Diamond : public Movable {
    private:
        static const int DIAMOND_VELOCITY = 3;


    public:
        const static std::string DIAMOND_SHAPE;

        Diamond(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;
};

#endif // DIAMOND_HPP