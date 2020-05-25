#include "level/tile.hpp"

Tile::Tile(int mapX, int mapY, int tileWidth, int tileHeight, int tilesetFirstGID,
        int tilesetGID, int tileParity) : x(mapX), y(mapY), 
        tilesetFirstGID(tilesetFirstGID), tilesetGID(tilesetGID), 
        tileParity(tileParity) {
    renderArea = {mapX, mapY, tileWidth, tileHeight};
}

void Tile::render(SDL_Renderer * renderer, SDL_Texture * tilesetTexture, 
        const SDL_Rect & tilesheetClip) const {
    SDL_RenderCopy(renderer, tilesetTexture, &tilesheetClip, &renderArea);
}

// Filp tile's parity, update tilesheetClip
void Tile::flip() {
    tileParity = tileParity == PARITY_GRAY ? PARITY_PURPLE : PARITY_GRAY;
}

int Tile::getTilesetFirstGID() const {
    return tilesetFirstGID;
}

int Tile::getTilesetGID() const {
    return tilesetGID;
}

int Tile::getTileParity() const {
    return tileParity;
}