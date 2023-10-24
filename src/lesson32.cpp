#include "framework1.h"

int main(int argc, char** argv) {
  Init();

  std::string render_text("some text");
  SDL_Event e;
  bool quit = false;
  SDL_Rect IME_UI_position;
  IME_UI_position.x = 20;
  IME_UI_position.y = 20;
  SDL_SetTextInputRect(&IME_UI_position);

  SDL_StartTextInput();
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)) {
          SDL_SetClipboardText(render_text.c_str());
        } else if (e.key.keysym.sym == SDLK_v &&
                   (SDL_GetModState() & KMOD_CTRL)) {
          render_text = SDL_GetClipboardText();
        } else if (e.key.keysym.sym == SDLK_BACKSPACE && !render_text.empty()) {
          render_text.pop_back();
        }
      }

      // MUST SET HINT_IME_SHOW to enable IME support
      else if (e.type == SDL_TEXTINPUT) {
        const char* text = e.text.text;
        if (!((SDL_GetModState() & KMOD_CTRL) &&
              (text[0] == 'c' || text[0] == 'C' || text[0] == 'v' ||
               text[0] == 'V'))) {
          render_text += text;
        }
      }
    }

    UTexture text_texture;
    SDL_Color text_color = {0, 0, 0, 0XFF};
    text_texture.LoadFromText(render_text != "" ? render_text.c_str() : " ",
                              text_color);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    text_texture.Render((kScreenWidth - text_texture.GetWidth()) / 2,
                        (kScreenHeight - text_texture.GetHeight()) / 2);

    SDL_RenderPresent(gRenderer);
  }

  SDL_StopTextInput();
  Close();
  return 0;
}