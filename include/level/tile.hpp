#ifndef TILE_HPP
#define TILE_HPP

#include <SDL.h>

enum TileParity {
    PARITY_GRAY,    // 0 ~ gray
    PARITY_PURPLE   // 1 ~ purple
};

class Tile {
    private:
        int x, y;

        // First GID of the tileset the tile belongs to, and its own GID 
        int tilesetFirstGID, tilesetGID;

        int tileParity;   // indicate tile color (0:gray, 1:purple)

        SDL_Rect renderArea;     // where on screen to render
        
    public:
        Tile(int mapX, int mapY, int tileWidth, int tileHeight, 
            int tilesetFirstGID, int tilesetGID, int tileParity);

        void render(SDL_Renderer * renderer, SDL_Texture * tilesetTexture, 
            const SDL_Rect & tilesheetClip) const;

        // flip the tile parity
        void flip();

        int getTilesetFirstGID() const;
        int getTilesetGID() const;
        
        int getTileParity() const;
};

#endif