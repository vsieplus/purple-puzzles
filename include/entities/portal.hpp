// portal entity class

#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "entities/entity.hpp"

class Player;

class Portal : public Entity {
    private:
        bool activated = false;
        bool removed = false;

        enum PortalAnimation {PORTAL_MERGE};

        // the corresponding portal
        std::shared_ptr<Portal> otherPortal;

        // the player being teleported by this portal
        std::shared_ptr<Player> player;

        void resetPortalStatus(Level * level);

    public:
        Portal(int screenX, int screenY, int gridX, int gridY, int parity,
            std::shared_ptr<Sprite> entitySprite,
            const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations);

        void handleEvents(const Uint8 * keyStates, Level * level) override;
        void update(Level * level, float delta) override;
        void render(SDL_Renderer* renderer) const override;

        // check if surrounded by purple tiles -> merge animation
        void checkSurrounded(Level * level, bool playerTeleported = true);

        // teleport the player
        void teleportPlayer(Level * level, bool undo = false);

        // temporarily lift portals from grid
        void removePortals(Level * level);

        void setPlayer(std::shared_ptr<Player> player);
        void setOtherPortal(std::shared_ptr<Portal> otherPortal);

        std::shared_ptr<Player> getPlayer() const;

        void setRemoved(bool removed);
        bool isRemoved() const;
        void setActivated(bool activated);
        bool isActivated() const;
};

#endif // PORTAL_HPP