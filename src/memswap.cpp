/* File: memswap.cpp
 * Desc: driver class for memswap 
 */

#include "memswap.hpp"

/**
 * @brief initialize SDL window/renderer
 * 
 * @param window ref. to pointer to SDL_Window to init
 * @param renderer ref. to pointer to SDL_Renderer to init
 * @return true if successful
 * @return false if unsuccessful
 */
bool init (SDL_Window *& window, SDL_Renderer *& renderer) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
		return false;
	} 

	// Try to create window
	window = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	return true;
}

/**
 * @brief initialize SDL libraries we're using
 * 
 * @return true if successful
 * @return false if unsuccessful
 */
bool initLibs() {
	// Initialize SDL_image
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_Image Error: %s\n", IMG_GetError());
		return false;
	}

	// Init SDL_ttf
	if(TTF_Init() == -1) {
		printf("SDL_ttf error: %s\n", TTF_GetError());
		return false;
	}

	// Initialize Mixer
	if(Mix_OpenAudio(SOUND_FREQ, MIX_DEFAULT_FORMAT, NUM_CHANNELS, 
		SAMPLE_SIZE) < 0) {
		printf("SDL_mixer error %s\n", Mix_GetError());
		return false;
	}

	return true;
}

/**
 * @brief quit SDL components and free resources
 *
 * @param window pointer to SDL_Window to init
 * @param renderer pointer to SDL_Renderer to init
 */
void close(SDL_Window *& window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

/**
 * @brief main method
 * 
 */
int main(int argc, char* args[]) {
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    
    if(!(init(window, renderer) && initLibs())) {
        printf("Failed to initialize SDL");
        return -1;
    }


	close(window, renderer);
     return 0;
}