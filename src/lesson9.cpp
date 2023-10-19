
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common_utils.h"

void Init();
void LoadMedia();
void Close();

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;

SDL_Window *gWindow = nullptr;
SDL_Texture *gTexture = nullptr;
SDL_Renderer *gRenderer = nullptr;

SDL_Surface *LoadMedia(const char *path) {
  SDL_Surface *p = IMG_Load(path);
  check_error_img(p == nullptr);
  return p;
}

SDL_Texture *LoadTexture(const char *path) {
  SDL_Surface *img = LoadMedia(path);
  SDL_Texture *p = SDL_CreateTextureFromSurface(gRenderer, img);
  SDL_FreeSurface(img);
  check_error(p == nullptr);
  return p;
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
  check_error(gRenderer == nullptr);
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  ensure_img(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
  gTexture = LoadTexture("images/viewport.png");
}

void Close() {
  SDL_DestroyTexture(gTexture);
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
    SDL_RenderClear(gRenderer);

    SDL_Rect viewport1 = {
        0,
        0,
        kScreenWidth / 2,
        kScreenHeight / 2,
    };

    SDL_RenderSetViewport(gRenderer, &viewport1);
    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

    SDL_Rect viewport2 = {
        kScreenWidth / 2,
        0,
        kScreenWidth / 2,
        kScreenHeight / 2,
    };
    SDL_RenderSetViewport(gRenderer, &viewport2);
    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

    SDL_Rect viewport3 = {
        0,
        kScreenHeight / 2,
        kScreenWidth,
        kScreenHeight / 2,
    };
    SDL_RenderSetViewport(gRenderer, &viewport3);
    SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

    SDL_RenderPresent(gRenderer);
  }
  Close();
  return 0;
}