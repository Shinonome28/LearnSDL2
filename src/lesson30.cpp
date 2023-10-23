#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;

  UDot dot(10, 10);
  dot.ShiftTo(10, 10);
  UTexture bg;
  bg.LoadFromFile("images/bg.png");

  SDL_Rect camera = {0, 0, kScreenWidth, kScreenHeight};

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      dot.HandleEvent(e, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
    }

    dot.MoveInLevel();

    camera.x = (dot.GetXPosition() + kDotWidth / 2) - kScreenWidth / 2;
    camera.y = (dot.GetYPosition() + kDotHeight / 2) - kScreenHeight / 2;
    cap(camera.x, 0, kLevelWidth - camera.w);
    cap(camera.y, 0, kLevelHeight - camera.h);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    bg.Render(0, 0, &camera);
    dot.RenderRelativeToCamera(camera);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}