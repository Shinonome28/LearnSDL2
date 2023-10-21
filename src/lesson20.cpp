#include "framework1.h"

int main(int argc, char** argv) {
  Init();
  InitGameController();

  UTexture splash;
  splash.LoadFromFile("images/splash.png");

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_JOYBUTTONDOWN) {
        if (gGameController != nullptr) {
          SDL_GameControllerRumble(gGameController, 0xFFFF * 3 / 4,
                                   0xFFFF * 3 / 4, 500);
        } else if (gJoyHaptic != nullptr) {
          SDL_HapticRumblePlay(gJoyHaptic, 0.75, 500);
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    splash.Render(0, 0);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}