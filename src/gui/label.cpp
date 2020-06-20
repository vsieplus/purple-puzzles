#include "gui/label.hpp"

Label::Label(int screenX, int screenY, std::shared_ptr<Texture> labelSprite) :
    Label(screenX, screenY, labelSprite, nullptr, "", (SDL_Color) {0, 0, 0, 0}){}

Label::Label(int screenX, int screenY, std::shared_ptr<Texture> labelSprite,
    std::shared_ptr<BitmapFont> labelFont, std::string labelText,  SDL_Color textColor,
    TextAlignment horizTextAlignment, TextAlignment vertTextAlignment) : 
    labelSprite(labelSprite), labelText(labelText), labelFont(labelFont), 
    textColor(textColor),
    horizTextAlignment(horizTextAlignment),
    vertTextAlignment(vertTextAlignment),
    screenX(screenX), screenY(screenY),
    textX(initTextX()),
    textY(initTextY()),
    hasText(labelFont != nullptr && labelText.length() > 0) {}

int Label::initTextX() const {
    int x = 0;

    // center/left/right align text
    if(labelFont.get()) {
        switch(horizTextAlignment) {
            case TextAlignment::ALIGN_LEFT:
                x = screenX + labelSprite->getWidth() / 12;
                break;
            case TextAlignment::ALIGN_CENTER:
                x = screenX + labelSprite->getWidth() / 2 -
                    labelFont->getTextWidth(labelText) / 2;
                break;
            case TextAlignment::ALIGN_RIGHT:
                x = screenX + labelSprite->getWidth() * 2 / 4;
                break;
        }
    }

    return x;
}

int Label::initTextY() const {
    int y = 0;

    if(labelFont.get()) {
        switch(vertTextAlignment) {
            case ALIGN_CENTER:
                y = screenY + labelSprite->getHeight() / 2 - 
                    labelFont->getTextHeight(labelText) / 2;
                break;
            case ALIGN_TOP:
                y = screenY + TEXT_PAD;
                break;
            case ALIGN_RIGHT:
                y = screenY + labelSprite->getHeight() - TEXT_PAD;
                break;
        }
    }

    return y;
}

void Label::render(SDL_Renderer * renderer) const {
    labelSprite->render(screenX, screenY, renderer);

    if(hasGraphic) {
        graphicSprite->render(screenX, screenY, renderer);
    } else if(hasText) {
        labelFont->setFontColor(textColor);
        labelFont->renderText(renderer, labelText, textX, textY);
    }
}

std::string Label::getText() const {
    return labelText;
}

void Label::setText(std::string text) {
    labelText = text;
    hasText = text.length() > 0;
}

int Label::getScreenX() const {
    return screenX;
}

int Label::getScreenY() const {
    return screenY;
}

int Label::getWidth() const {
    return labelSprite->getWidth();
}

int Label::getHeight() const {
    return labelSprite->getHeight();
}

bool Label::checkHasGraphic() const {
    return hasGraphic;
}

void Label::removeGraphic() {
    graphicSprite.reset();
    hasGraphic = false;
}

void Label::setGraphic(std::shared_ptr<Texture> graphicSprite) {
    this->graphicSprite = graphicSprite;
    hasGraphic = true;
}
