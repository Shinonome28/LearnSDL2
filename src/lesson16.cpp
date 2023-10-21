
#include "framework1.h"

int main(int argc, char **argv) {
  Init();

  UTexture text;
  SDL_Color text_color = {0, 0, 0};
  text.LoadFromText("The quick brown fox jumps over the lazy dog", text_color);

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    text.Render((kScreenWidth - text.GetWidth()) / 2,
                ((kScreenHeight - text.GetHeight())) / 2);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}
