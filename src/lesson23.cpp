#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  SDL_Color text_color = {0, 0, 0};
  UTexture gPromptTexture1, gPromptTexture2, gTimeTextTexture;
  gPromptTexture1.LoadFromText("Press Enter to start or stop timer",
                               text_color);
  gPromptTexture2.LoadFromText("Press Esc to pause timer or resume",
                               text_color);

  SDL_Event e;
  bool quit = false;
  UTimer timer;
  timer.Start();

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_RETURN ||
            e.key.keysym.sym == SDLK_KP_ENTER) {
          if (timer.CanStart()) {
            timer.Start();
          } else if (timer.CanStop()) {
            timer.Stop();
          }
        } else if (e.key.keysym.sym == SDLK_ESCAPE) {
          if (timer.CanPause()) {
            timer.Pause();
          } else if (timer.CanUnpause()) {
            timer.Unpause();
          }
        }
      }
    }

    std::stringstream buf;
    buf << "Milliseconds since start time " << timer.GetTime() / 1000.f;
    gTimeTextTexture.LoadFromText(buf.str().c_str(), text_color);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    gPromptTexture1.Render((kScreenWidth - gPromptTexture1.GetWidth()) / 2, 0);
    gPromptTexture2.Render((kScreenWidth - gPromptTexture2.GetWidth()) / 2,
                           gPromptTexture2.GetHeight());
    gTimeTextTexture.Render((kScreenWidth - gTimeTextTexture.GetWidth()) / 2,
                            (kScreenHeight - gTimeTextTexture.GetHeight()) / 2);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}