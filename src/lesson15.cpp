
#include "framework1.h"

int main(int argc, char **argv) {
  Init();

  UTexture texture;
  texture.LoadFromFile("images/arrow.png");
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  SDL_Event e;
  bool quit = false;
  double degrees = 0.0;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_1:
            flip = SDL_FLIP_HORIZONTAL;
            break;
          case SDLK_2:
            flip = SDL_FLIP_VERTICAL;
            break;
          case SDLK_3:
            degrees -= 30;
            break;
          case SDLK_4:
            degrees += 30;
            break;
          default:
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    TextureRenderOptions options;
    options.angle = degrees;
    options.flip = flip;
    texture.Render((kScreenWidth - texture.GetWidth()) / 2,
                   (kScreenHeight - texture.GetHeight()) / 2, &options);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}
