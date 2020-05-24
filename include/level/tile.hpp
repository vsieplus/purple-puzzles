#ifndef TILE_HPP
#define TILE_HPP

#include <memory>

#include <SDL.h>

class Tile {
    private:
        int x, y, width, height;

        SDL_Rect tilesheetClip;      // where on tilesheet texture to render
        SDL_Rect renderArea;         // where on screen to render

        // pointer to the right tilesheet texture
        std::shared_ptr<SDL_Texture> tilesetTexture; 
        
    public:
        Tile(int mapX, int mapY, int tilesetX, int tilesetY, int width,
             int height, SDL_Texture* texture);

        void render(SDL_Renderer* renderer);
};

#endif