#include <sstream>

#include "framework1.h"

constexpr int kFPS = 60;
constexpr int kFrameTime = 1000 / kFPS;

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;

  UTimer timer;
  timer.Start();
  UTimer capTimer;
  unsigned frame_count = 0;

  while (!quit) {
    capTimer.Start();
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    std::stringstream buf;
    buf << "The Average FPS is " << frame_count / (timer.GetTime() / 1000.0f);
    UTexture texture;
    SDL_Color text_color = {0, 0, 0};
    texture.LoadFromText(buf.str().c_str(), text_color);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    texture.Render(0, 0);

    SDL_RenderPresent(gRenderer);

    frame_count++;
    if (capTimer.GetTime() < kFrameTime) {
      SDL_Delay(kFrameTime - capTimer.GetTime());
    }
    capTimer.Stop();
  }

  Close();
  return 0;
}