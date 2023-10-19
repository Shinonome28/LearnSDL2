
#include <SDL2/SDL.h>

#include "common_utils.h"

void Init();
void LoadMedia();
void Close();

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;
constexpr int kTotalPressSurfaces = 6;

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

SDL_Window *gWindow = nullptr;
SDL_Surface *gWindowSurface = nullptr;
SDL_Surface **gKeyPressSurfaces = nullptr;
SDL_Surface *gCurrentSurface = nullptr;

SDL_Surface *LoadMedia(const char *path) {
  SDL_Surface *p = SDL_LoadBMP(path);
  check_error(p == nullptr);
  SDL_Surface *converted = SDL_ConvertSurface(p, gWindowSurface->format, 0);
  check_error(converted == nullptr);
  SDL_FreeSurface(p);
  return converted;
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);
  gWindowSurface = SDL_GetWindowSurface(gWindow);
  gKeyPressSurfaces = new SDL_Surface *[kTotalPressSurfaces];
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadMedia("images/press.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadMedia("images/up.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadMedia("images/down.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadMedia("images/left.bmp");
  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadMedia("images/right.bmp");
}

void Close() {
  for (int i = 0; i < kTotalPressSurfaces; i++) {
    SDL_FreeSurface(gKeyPressSurfaces[i]);
  }
  delete[] gKeyPressSurfaces;
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
}

int main(int argc, char **argv) {
  Init();

  SDL_Event e;
  bool quit = false;
  gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      else if (e.type == SDL_KEYDOWN) {
        // Select surfaces based on key press
        switch (e.key.keysym.sym) {
          case SDLK_UP:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
            break;

          case SDLK_DOWN:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
            break;

          case SDLK_LEFT:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
            break;

          case SDLK_RIGHT:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
            break;

          default:
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            break;
        }
      }
      SDL_Rect stretchRect;
      stretchRect.x = 0;
      stretchRect.y = 0;
      stretchRect.h = kScreenHeight;
      stretchRect.w = kScreenWidth;

      SDL_BlitScaled(gCurrentSurface, NULL, gWindowSurface, &stretchRect);
      SDL_UpdateWindowSurface(gWindow);
    }
  }
  Close();
  return 0;
}