// Header file for memswap.cpp

#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// Window constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const std::string GAME_TITLE = "Memory Swap";

// Audio
const int SOUND_FREQ = 44100;
const int NUM_CHANNELS = 2;
const int SAMPLE_SIZE = 2048;

/**
 * @brief initialize SDL window and renderer
 * 
 */
bool init(SDL_Window *& window, SDL_Renderer *& renderer);

/**
 * @brief initialize SDL libararies we're using
 * 
 */
bool initLibs();

/**
 * @brief quit SDL components and free resources
 * 
 */
void quit(SDL_Window *& window, SDL_Renderer *& renderer);