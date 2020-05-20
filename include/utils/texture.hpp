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
        SDL_Texture* texture;

        int width, height;

    public:
        // constructor initializing texture from an image path
        Texture(std::string path, SDL_Renderer* renderer);

        // constructor initializing textual texture from an input string/font
        Texture(std::string textureText, SDL_Color textColor, TTF_Font * font, 
            SDL_Renderer* renderer);

        ~Texture();

        // free old texture
        void freeTexture();

        // set texture color
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        // blending
        void setBlendMode(SDL_BlendMode blending);

        // transparency
        void setAlpha(Uint8 alpha);

        /**
         * @brief   Render the texture at position x,y to the given renderer
         * If provided, renders a specific clip of the texture, rotated at a 
         * particular angle/flipped
         */
        void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL,
            double angle = 0.0, SDL_Point* center = NULL,
            SDL_RendererFlip flip = SDL_FLIP_NONE);

        int getHeight();
        int getWidth();

};

#endif // TEXTURE_HPP