#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;

  UDot dot(5, 5);
  dot.ShiftTo(10, 10);
  UTexture bg;
  bg.LoadFromFile("images/bg2.png");
  int scroll_offset = 0;

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

    SDL_Rect clip1 = {scroll_offset, 0, bg.GetWidth() - scroll_offset,
                      kScreenHeight};
    bg.Render(0, 0, &clip1);
    SDL_Rect clip2 = {0, 0, bg.GetWidth(), kScreenHeight};
    bg.Render(bg.GetWidth() - scroll_offset, 0, &clip2);
    bg.Render(2 * bg.GetWidth() - scroll_offset, 0, &clip2);
    dot.Render();

    SDL_RenderPresent(gRenderer);

    scroll_offset++;
    if (scroll_offset >= bg.GetWidth()) {
      scroll_offset = 0;
    }
  }

  Close();
  return 0;
}