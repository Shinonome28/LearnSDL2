#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;

  UDot dot(5, 5), other_dot(5, 5);
  dot.ShiftTo(kScreenWidth / 4, kScreenHeight / 4);

  SDL_Rect wall;
  wall.x = 300;
  wall.y = 40;
  wall.w = 40;
  wall.h = 400;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      dot.HandleEvent(e, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
      other_dot.HandleEvent(e, SDLK_KP_5, SDLK_KP_2, SDLK_KP_1, SDLK_KP_3);
    }

    dot.MoveInSceneUseCircleCollider(wall, other_dot.GetCircleCollider());
    other_dot.MoveInSceneUseCircleCollider(wall, dot.GetCircleCollider());

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    dot.Render();
    other_dot.Render();

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(gRenderer, &wall);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}