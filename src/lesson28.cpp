#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;

  UDot first(10, 10);
  first.ShiftTo(0, 0);
  UDot second(10, 10);
  second.ShiftTo(kScreenWidth / 4, kScreenHeight / 4);

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      first.HandleEvent(e, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
      second.HandleEvent(e, SDLK_KP_5, SDLK_KP_2, SDLK_KP_1, SDLK_KP_3);
    }

    first.Move(second.GetColliders());
    second.Move(first.GetColliders());

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    first.Render();
    second.Render();

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}