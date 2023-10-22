
#include <sstream>

#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Color text_color = {0, 0, 0};
  UTexture gPromptTexture, gTimeTextTexture;
  gPromptTexture.LoadFromText("Press enter to restart timer", text_color);

  SDL_Event e;
  bool quit = false;
  Uint32 start_ticks = 0;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_RETURN ||
            e.key.keysym.sym == SDLK_KP_ENTER) {
          start_ticks = SDL_GetTicks();
        }
      }
    }

    std::stringstream buf;
    buf << "Milliseconds since start time " << SDL_GetTicks() - start_ticks;
    gTimeTextTexture.LoadFromText(buf.str().c_str(), text_color);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    gPromptTexture.Render((kScreenWidth - gPromptTexture.GetWidth()) / 2, 0);
    gTimeTextTexture.Render((kScreenWidth - gTimeTextTexture.GetWidth()) / 2,
                            (kScreenHeight - gPromptTexture.GetHeight()) / 2);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}