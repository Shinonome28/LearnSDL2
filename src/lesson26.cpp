#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;
  UDot dot(10, 10);

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      dot.HandleEvent(e, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
    }

    dot.Move();

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    dot.Render();
    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}