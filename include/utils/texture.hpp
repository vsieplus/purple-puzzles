// Header file for Texture Texture wrapper class
 
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stdio.h>
#include <string>
#include <exception>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Exception class for texture loading
class TextureLoadException : public std::exception {
    private:
        std::string message_;
    public:
        explicit TextureLoadException(const std::string& message) : 
            message_(message) { }
        virtual const char * what() const throw() {
            return message_.c_str();
        }
};

class Texture {
    private:
        // pointer to the SDL texture
        SDL_Texture * texture = NULL;

        int width = 0;
        int height = 0;

        // for texture transparency
        const static int ALPHA_MAX = 255;
        int textureAlpha = 255;

        bool alphaIncreasing = false;
    public:
        Texture();
        ~Texture();

        // initializing texture from an image path
        void loadTexture(std::string path, SDL_Renderer* renderer);

        // initializing textual texture from an input string/font
        void loadTextTexture(std::string textureText, SDL_Color textColor, TTF_Font * font, 
            SDL_Renderer* renderer);

        // free old texture
        void freeTexture();

        // set texture color
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        // blending
        void setBlendMode(SDL_BlendMode blending);

        // transparency
        void updateAlpha();             // flashing effect
        void setAlpha(Uint8 alpha);     // one-time update

        /**
         * @brief   Render the texture at position x,y to the given renderer
         * If provided, renders a specific clip of the texture, rotated at a 
         * particular angle/flipped
         */
        void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL,
            double angle = 0.0, SDL_Point* center = NULL,
            SDL_RendererFlip flip = SDL_FLIP_NONE) const;

        int getHeight();
        int getWidth();
};

#endif // TEXTURE_HPP