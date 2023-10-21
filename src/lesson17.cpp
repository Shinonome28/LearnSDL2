#include "framework1.h"

UTexture gButtonTexture;
SDL_Rect gSpriteClips[4];
UButton* gButtons[4];
constexpr int kButtonWidth = 300;
constexpr int kButtonHeight = 200;

int main(int argc, char** argv) {
  Init();

  gButtonTexture.LoadFromFile("images/button.png");

  for (int i = 0; i < 4; ++i) {
    gSpriteClips[i].x = 0;
    gSpriteClips[i].y = i * kButtonHeight;
    gSpriteClips[i].w = kButtonWidth;
    gSpriteClips[i].h = kButtonHeight;
  }

  gButtons[0] = new UButton(0, 0, kButtonWidth, kButtonHeight);
  gButtons[1] =
      new UButton(kScreenWidth - kButtonWidth, 0, kButtonWidth, kButtonHeight);
  gButtons[2] = new UButton(0, kScreenHeight - kButtonHeight, kButtonWidth,
                            kButtonHeight);
  gButtons[3] =
      new UButton(kScreenWidth - kButtonWidth, kScreenHeight - kButtonHeight,
                  kButtonWidth, kButtonHeight);

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      for (int i = 0; i < 4; i++) {
        gButtons[i]->HandleEvent(&e);
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < 4; i++) {
      switch (gButtons[i]->GetState()) {
        case BUTTON_MOUSE_OUT:
        case BUTTON_MOUSE_UNKNOWN:
          gButtonTexture.Render(gButtons[i]->GetX(), gButtons[i]->GetY(),
                                &gSpriteClips[0]);
          break;
        case BUTTON_MOUSE_OVER_MOTION:
          gButtonTexture.Render(gButtons[i]->GetX(), gButtons[i]->GetY(),
                                &gSpriteClips[1]);
          break;
        case BUTTON_MOUSE_DOWN:
          gButtonTexture.Render(gButtons[i]->GetX(), gButtons[i]->GetY(),
                                &gSpriteClips[2]);
          break;
        case BUTTON_MOUSE_UP:
          gButtonTexture.Render(gButtons[i]->GetX(), gButtons[i]->GetY(),
                                &gSpriteClips[3]);
          break;
      }
    }

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}