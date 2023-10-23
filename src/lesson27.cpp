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

    SDL_Rect wall;
    wall.x = 300;
    wall.y = 40;
    wall.w = 40;
    wall.h = 400;

    dot.Move(wall);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &wall);

    dot.Render();
    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}