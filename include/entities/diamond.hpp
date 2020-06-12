// movable diamond object

#ifndef DIAMOND_HPP
#define DIAMOND_HPP

#include "entities/movable.hpp"
#include "entities/receptor.hpp"

class Diamond : public Movable {
    private:
        static const int DIAMOND_VELOCITY = 5;

        // if merging with a receptor
        bool merging = false;

        // the receptor to merge with
        std::shared_ptr<Receptor> dReceptor;

    public:
        const static std::string DIAMOND_SHAPE;

        Diamond(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        void checkReceptor(Level * level, Direction direction);

        bool isMerging() const;
};

#endif // DIAMOND_HPP