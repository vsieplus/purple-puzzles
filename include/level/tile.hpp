#ifndef TILE_HPP
#define TILE_HPP

#include <SDL.h>

class Level;

enum TileParity {
    PARITY_NONE,    // 0 ~ none
    PARITY_GRAY,    // 1 ~ gray
    PARITY_PURPLE   // 2 ~ purple
};

class Tile {
    private:
        // x, y value on screen
        int x, y;

        // First GID of the tileset the tile belongs to, and its own GID 
        int tilesetFirstGID, tilesetGID;

        int tileParity;          // indicate tile color (0:gray, 1:purple)
                                 // (-1 for no parity)

        bool flipped = false;

        SDL_Rect renderArea;     // where on screen to render
        
    public:
        Tile(int mapX, int mapY, int tileWidth, int tileHeight, int tilesetFirstGID, 
            int tilesetGID, int tileParity);

        void update(Level * level);

        void render(SDL_Renderer * renderer, SDL_Texture * tilesetTexture, 
            const SDL_Rect & tilesheetClip) const;

        // flip the tile parity
        void flip(int newTilesetGID);

        int getTilesetFirstGID() const;
        int getTilesetGID() const;
        
        int getTileParity() const;

        bool isFlipped() const;
};

#endif