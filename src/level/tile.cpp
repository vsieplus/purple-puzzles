#include "Tile.hpp"

Tile::Tile(int mapX, int mapY, int tilesetX, int tilesetY, int width, int height, 
    std::shared_ptr<SDL_Texture> texture) : x(mapX), y(mapY), width(width), 
    height(height), tilesetTexture(texture) {

    tilesheetClip = {tilesetX, tilesetY, width, height};
    renderArea = {mapX, mapY, width, height};
}

void Tile::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, tilesetTexture, &tilesheetClip, &renderArea);
}