// Label gui element

#ifndef LABEL_HPP
#define LABEL_HPP

#include "utils/texture.hpp"
#include "utils/bitmapfont.hpp"

class Label {
    public:
        // enum for label text alignment
        enum TextAlignment {
            ALIGN_LEFT,         // left align
            ALIGN_CENTER,       // center text
            ALIGN_RIGHT,        // right align
            ALIGN_TOP,          // vertical (top)
            ALIGN_BOTTOM
        };

        // without text
        Label(int screenX, int screenY, std::shared_ptr<Texture> labelSprite);

        // with text
        Label(int screenX, int screenY, std::shared_ptr<Texture> labelSprite,
            std::shared_ptr<BitmapFont> labelFont, std::string label, 
            SDL_Color textColor, TextAlignment textAlignment = ALIGN_CENTER,
            TextAlignment vertTextAlignment = ALIGN_CENTER);

        void render(SDL_Renderer * renderer) const;

        void setText(std::string text);
        std::string getText() const;
        int getScreenX() const;
        int getScreenY() const;
        int getWidth() const;
        int getHeight() const;

        bool checkHasGraphic() const;

        void removeGraphic();
        void setGraphic(std::shared_ptr<Texture> graphicSprite);

    protected:
        std::shared_ptr<Texture> labelSprite;
        std::shared_ptr<Texture> graphicSprite;
        std::string labelText;
        std::shared_ptr<BitmapFont> labelFont;

        SDL_Color textColor;

        TextAlignment horizTextAlignment;
        TextAlignment vertTextAlignment;

        // pad for text from label border
        const static int TEXT_PAD = 15;

        // coords of the texture
        int screenX, screenY;

        // coords of the label
        int textX, textY;

        bool hasText;

        bool hasGraphic = false;

        int initTextX() const;
        int initTextY() const;
};

#endif //LABEL_HPP