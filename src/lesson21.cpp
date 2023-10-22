#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  Mix_Music* gMusic = LoadMusic("sounds/beat.wav");

  Mix_Chunk* gScratch = LoadChunk("sounds/scratch.wav");
  Mix_Chunk* gHigh = LoadChunk("sounds/high.wav");
  Mix_Chunk* gMedium = LoadChunk("sounds/medium.wav");
  Mix_Chunk* gLow = LoadChunk("sounds/low.wav");

  UTexture prompt;
  prompt.LoadFromFile("images/prompt.png");

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    const Uint8* current_keyboard_states = SDL_GetKeyboardState(nullptr);
    if (current_keyboard_states[SDL_SCANCODE_1] ||
        current_keyboard_states[SDL_SCANCODE_KP_1]) {
      // If we specify -1 it will use the whatever possible channel
      Mix_PlayChannel(-1, gLow, 0);
    } else if (current_keyboard_states[SDL_SCANCODE_2] ||
               current_keyboard_states[SDL_SCANCODE_KP_2]) {
      Mix_PlayChannel(-1, gMedium, 0);
    } else if (current_keyboard_states[SDL_SCANCODE_3] ||
               current_keyboard_states[SDL_SCANCODE_KP_3]) {
      Mix_PlayChannel(-1, gMedium, 0);
    } else if (current_keyboard_states[SDL_SCANCODE_4] ||
               current_keyboard_states[SDL_SCANCODE_KP_4]) {
      Mix_PlayChannel(-1, gHigh, 0);
    } else if (current_keyboard_states[SDL_SCANCODE_9] ||
               current_keyboard_states[SDL_SCANCODE_KP_9]) {
      Mix_PlayMusic(gMusic, -1);
    } else if (current_keyboard_states[SDL_SCANCODE_0] ||
               current_keyboard_states[SDL_SCANCODE_KP_0]) {
      Mix_HaltMusic();
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    prompt.Render(0, 0);
    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}