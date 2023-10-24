#include "framework1.h"

constexpr size_t kTotalData = 10;

int main(int argc, char** argv) {
  Init();

  Uint32 gData[kTotalData];
  UTexture gTextures[kTotalData];
  SDL_Color text_color = {0, 0, 0, 0xFF};
  SDL_Color highlight_color = {0, 0xFF, 0xFF, 0xFF};
  size_t highlight_index = 0;
  UTexture gPromptTexture;
  gPromptTexture.LoadFromText("Enter Data: ", text_color);

  SDL_RWops* file = SDL_RWFromFile("data/nums.bin", "r+b");
  if (file == nullptr) {
    file = SDL_RWFromFile("data/nums.bin", "w+b");
    check_error(file == nullptr);
    for (Uint32 i = 0; i < kTotalData; i++) {
      gData[i] = i;
      SDL_RWwrite(file, &i, sizeof(Uint32), 1);
    }
  } else {
    SDL_RWread(file, gData, sizeof(Uint32), kTotalData);
  }
  SDL_RWclose(file);

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP) {
          if (highlight_index > 0) {
            highlight_index--;
          } else {
            highlight_index = kTotalData - 1;
          }
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          if (highlight_index < kTotalData - 1) {
            highlight_index++;
          } else {
            highlight_index = 0;
          }
        } else if (e.key.keysym.sym == SDLK_LEFT) {
          if (gData[highlight_index] > 0) {
            gData[highlight_index]--;
          }
        } else if (e.key.keysym.sym == SDLK_RIGHT) {
          gData[highlight_index]++;
        }
      }
    }

    for (size_t i = 0; i < kTotalData; i++) {
      gTextures[i].TryFree();
      gTextures[i].LoadFromText(
          std::to_string(gData[i]).c_str(),
          i == highlight_index ? highlight_color : text_color);
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    int y_offset = 0;
    gPromptTexture.Render((kScreenWidth - gPromptTexture.GetWidth()) / 2,
                          y_offset);
    y_offset += gPromptTexture.GetHeight();

    for (size_t i = 0; i < kTotalData; i++) {
      gTextures[i].Render((kScreenWidth - gTextures[i].GetHeight()) / 2,
                          y_offset);
      y_offset += gTextures[i].GetHeight();
    }

    SDL_RenderPresent(gRenderer);
  }

  file = SDL_RWFromFile("data/nums.bin", "w+b");
  check_error(file == nullptr);
  SDL_RWwrite(file, gData, sizeof(Uint32), kTotalData);
  Close();
  return 0;
}