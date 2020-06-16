#include "gui/button.hpp"

Button::Button(int screenX, int screenY, bool clickable, 
    std::shared_ptr<Texture> buttonSprite, SDL_Color outlineColor) : 
    screenX(screenX), screenY(screenY), clickable(clickable),
    buttonSprite(buttonSprite), buttonOutline((SDL_Rect) {screenX - 1, 
    screenY - 1, buttonSprite->getWidth() + 2, buttonSprite->getHeight() + 2}), 
    outlineColor(outlineColor) {}

void Button::handleEvents(const SDL_Event & e) {
    if(clickable) {
        handleMouseEvents(e);
    } else {
        handleKeyEvents(e);    
    }
}

void Button::handleMouseEvents(const SDL_Event & e) {
    // check for mouse events
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ||
       e.type == SDL_MOUSEBUTTONUP) {
        
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // check if mouse is on button
        mouseOn = mouseX >= screenX && mouseX <= screenX + buttonSprite->getWidth() 
            && mouseY >= screenY && mouseY <= screenY + buttonSprite->getHeight();

        inFocus = mouseOn;
        
        if(mouseOn) {
            switch(e.type) {
                // check if button is clicked down/released
                case SDL_MOUSEBUTTONDOWN:
                    mouseDown = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseDown = false;
                    activated = true;
            }
        }    
    }
}

void Button::handleKeyEvents(const SDL_Event & e) {
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
        activated = true;
    }
}

void Button::render(SDL_Renderer * renderer) {
    buttonSprite->render(screenX, screenY, renderer);

    // if button is in focus, draw an outline around the button
    if(inFocus) {
        SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g,
            outlineColor.b, outlineColor.a);
        SDL_RenderDrawRect(renderer, &buttonOutline);
        
        // for mouse-controlled buttons, add additional effect on mouse-down
        if(mouseDown) {

        }
    }
}

void Button::setFocus(bool focus) {
    inFocus = focus;
}

bool Button::isActivated() const {
    return activated;
}