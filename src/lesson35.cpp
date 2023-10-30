#include "framework1.h"

int main(int argc, char** argv) {
  Init(false);

  UWindow window;
  window.Init();
  SDL_Renderer* renderer = window.CreateRenderer();

  UTexture bg;
  bg.LoadFromFile(renderer, "images/window.png");

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      window.HandleEvent(e);
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    bg.Render(renderer, (kScreenWidth - bg.GetWidth()) / 2,
              (kScreenHeight - bg.GetHeight()) / 2, nullptr);

    SDL_RenderPresent(renderer);
  }

  window.TryFree();
  Close();

  return 0;
}