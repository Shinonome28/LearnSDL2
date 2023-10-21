#include <cmath>

#include "framework1.h"
using namespace std;

constexpr int kDeadZone = 0;

int main(int argc, char** argv) {
  Init();
  SDL_Joystick* gJoyStick = nullptr;
  ensure_custom(SDL_NumJoysticks() >= 1, "Not found joystick");
  gJoyStick = SDL_JoystickOpen(0);
  ensure(gJoyStick != nullptr);

  UTexture arrow_texture;
  arrow_texture.LoadFromFile("images/arrow.png");

  SDL_Event e;
  bool quit = false;
  Sint16 xDir = 0, yDir = 0;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_JOYAXISMOTION) {
        // debug() << std::to_string(e.jaxis.axis) << " " << e.jaxis.value
        //         << debug_end();
        if (e.jaxis.which == 0) {
          if (e.jaxis.axis == 0) {
            xDir = e.jaxis.value;
          } else if (e.jaxis.axis == 1) {
            yDir = e.jaxis.value;
          }
        }
      }
    }

    // notice that SDL use degrees not radians
    double joystick_angle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
    // for atan2 if x & y is both 0 the returned value is undefined
    if (xDir == 0 && yDir == 0) {
      joystick_angle = 0;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    TextureRenderOptions options;
    options.angle = joystick_angle;
    arrow_texture.Render((kScreenWidth - arrow_texture.GetWidth()) / 2,
                         (kScreenHeight - arrow_texture.GetHeight()) / 2,
                         &options);

    SDL_RenderPresent(gRenderer);
  }
  SDL_JoystickClose(gJoyStick);
  Close();
  return 0;
}