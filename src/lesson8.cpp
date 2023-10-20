
#include <SDL2/SDL.h>

#include "common_utils.h"

void Init();
void LoadMedia();
void Close();

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  check_error(gRenderer == nullptr);
}

void Close() {
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
}

int main(int argc, char **argv) {
  Init();

  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_Rect filled_rect = {
        kScreenWidth / 4,
        kScreenHeight / 4,
        kScreenWidth / 2,
        kScreenHeight / 2,
    };
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(gRenderer, &filled_rect);

    SDL_Rect outlined_rect = {kScreenWidth / 6, kScreenHeight / 6,
                              kScreenWidth / 3 * 2, kScreenHeight / 3 * 2};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &outlined_rect);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(gRenderer, 0, kScreenHeight / 2, kScreenWidth,
                       kScreenHeight / 2);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0, 0xFF);
    for (int i = 0; i < kScreenHeight; i += 4) {
      SDL_RenderDrawPoint(gRenderer, kScreenWidth / 2, i);
    }
    SDL_RenderPresent(gRenderer);
  }
  Close();
  return 0;
}