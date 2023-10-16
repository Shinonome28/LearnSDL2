#include <SDL2/SDL.h>

#include <cstdio>

#include "common_utils.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char **argv) {
  SDL_Window *window = nullptr;
  SDL_Surface *screen_surface = nullptr;

  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  check_error(window == nullptr);
  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillRect(screen_surface, NULL,
               SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }
  }

  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}