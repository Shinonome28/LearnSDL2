#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common_utils.h"

int main(int argc, char **argv) {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  check_error_img(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG));

  SDL_Window *window;
  window =
      SDL_CreateWindow("SDL2 demo", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  SDL_Surface *window_surface = SDL_GetWindowSurface(window);

  check_error(window == nullptr);
  SDL_Surface *image;
  image = IMG_Load("images/loaded.png");
  check_error_img(image == nullptr);
  SDL_Surface *converted = SDL_ConvertSurface(image, window_surface->format, 0);
  check_error(converted == nullptr);
  SDL_FreeSurface(image);

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.h = 600;
    stretchRect.w = 800;

    SDL_BlitScaled(converted, NULL, window_surface, &stretchRect);
    SDL_UpdateWindowSurface(window);
  }
  SDL_FreeSurface(converted);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}