#include "framework1.h"

int main(int argc, char **argv) {
  Init();

  UTexture up_texture, down_texture, left_texture, right_texture,
      default_texture;
  up_texture.LoadFromFile("images/up.png");
  down_texture.LoadFromFile("images/down.png");
  left_texture.LoadFromFile("images/left.png");
  right_texture.LoadFromFile("images/right.png");
  default_texture.LoadFromFile("images/press.png");
  UTexture *current_texture = &default_texture;

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

    const Uint8 *current_keyboard_states = SDL_GetKeyboardState(nullptr);
    if (current_keyboard_states[SDL_SCANCODE_UP]) {
      current_texture = &up_texture;
    } else if (current_keyboard_states[SDL_SCANCODE_DOWN]) {
      current_texture = &down_texture;
    } else if (current_keyboard_states[SDL_SCANCODE_LEFT]) {
      current_texture = &left_texture;
    } else if (current_keyboard_states[SDL_SCANCODE_RIGHT]) {
      current_texture = &right_texture;
    }
    current_texture->Render(0, 0);

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}