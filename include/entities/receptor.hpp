// Receptor object

#ifndef RECEPTOR_HPP
#define RECEPTOR_HPP

#include "entities/entity.hpp"

class Receptor : public Entity {
    private:
        // track if receptor is completed
        bool completed = false;

        std::string shape;

    public:
        Receptor(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite, std::string shape);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        void setCompleted(bool completed);
        bool isCompleted() const;

        std::string getShape() const;
};

#endif // RECEPTOR_HPP