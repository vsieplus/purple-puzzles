// portal entity class

#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "entities/entity.hpp"

class Player;

class Portal : public Entity {
    private:
        bool activated = false;
        bool removed = false;

        // the corresponding portal
        std::shared_ptr<Portal> otherPortal;

        // the player being teleported by this portal
        std::shared_ptr<Player> player;

    public:
        Portal(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        // teleport the player
        void teleportPlayer(Level * level);

        // temporarily lift portals from grid
        void removePortals(Level * level);

        void setPlayer(std::shared_ptr<Player> player);
        void setOtherPortal(std::shared_ptr<Portal> otherPortal);

        void setRemoved(bool removed);
        bool isRemoved() const;
        void setActivated(bool activated);
        bool isActivated() const;
};

#endif // PORTAL_HPP