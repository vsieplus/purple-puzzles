// class for button gui element

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <memory>
#include <SDL.h>

#include "utils/texture.hpp"

class Button {
    public:
        Button(int screenX, int screenY, bool clickable,
            std::shared_ptr<Texture> buttonSprite, SDL_Color outlineColor);

        // mouse or keyboard events (depending on GUI type)
        void handleEvents(const SDL_Event & e);
        void handleMouseEvents(const SDL_Event & e);
        void handleKeyEvents(const SDL_Event & e);

        void update();
        void render(SDL_Renderer * renderer);

        void setFocus(bool focus);

        bool isActivated() const;

    private:
        // position on screen
        int screenX, screenY;

        // if button is part of clickable gui (or gamepad controlled focus)
        bool clickable;

        // if the button is in gui focus
        bool inFocus = false;

        // whether the mouse is over/on the button + down or not
        bool mouseOn = false;
        bool mouseDown = false;

        // use this to determine when to take action
        bool activated = false;

        std::shared_ptr<Texture> buttonSprite;

        // button outline
        SDL_Rect buttonOutline;

        // outline color
        SDL_Color outlineColor;
};

#endif // BUTTON_HPP