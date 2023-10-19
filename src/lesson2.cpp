
#include <SDL2/SDL.h>

#include "common_utils.h"

void Init();
void LoadMedia();
void Close();

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;

SDL_Window *gWindow = nullptr;
SDL_Surface *gWindowSurface = nullptr;
SDL_Surface *gHelloWorld = nullptr;

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);
  gWindowSurface = SDL_GetWindowSurface(gWindow);
}

void LoadMedia() {
  gHelloWorld = SDL_LoadBMP("images/hello_world.bmp");
  check_error(gHelloWorld == nullptr);
}

void Close() {
  SDL_FreeSurface(gHelloWorld);
  SDL_DestroyWindow(gWindow);

  SDL_Quit();
}

int main(int argc, char **argv) {
  Init();
  LoadMedia();
  SDL_BlitSurface(gHelloWorld, NULL, gWindowSurface, NULL);
  SDL_UpdateWindowSurface(gWindow);

  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }
  }
  Close();
  return 0;
}