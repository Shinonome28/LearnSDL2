#include <SDL2/SDL.h>

#include "common_utils.h"

class UTexture {
 public:
  void Free();
  void LoadFromFile(const char* path);

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

  void Render(int x, int y, const SDL_Rect* clip = nullptr);
  void SetColor(Uint8 Red, Uint8 Green, Uint8 Blue);

  ~UTexture() { Free(); }

 private:
  int width_;
  int height_;
  SDL_Texture* texture_ = nullptr;
};

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void UTexture::Free() {
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
  }
}

SDL_Surface* LoadMedia(const char* path) {
  SDL_Surface* p = IMG_Load(path);
  check_error_img(p == nullptr);
  return p;
}

void UTexture::LoadFromFile(const char* path) {
  SDL_Surface* img = LoadMedia(path);
  SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 0xFF, 0xFF));
  texture_ = SDL_CreateTextureFromSurface(gRenderer, img);
  check_error(texture_ == nullptr);
  width_ = img->w;
  height_ = img->h;
  SDL_FreeSurface(img);
}

void UTexture::Render(int x, int y, const SDL_Rect* clip) {
  SDL_Rect render_rect = {x, y, width_, height_};
  if (clip != nullptr) {
    render_rect.h = clip->h;
    render_rect.w = clip->w;
  }
  SDL_RenderCopy(gRenderer, texture_, clip, &render_rect);
}

void UTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(texture_, red, green, blue);
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
  check_error(gRenderer == nullptr);
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  ensure_img(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
}

void Close() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char** argv) {
  Init();

  UTexture texture;
  texture.LoadFromFile("images/colors.png");
  SDL_Event e;
  bool quit = false;
  Uint8 r = 255, g = 255, b = 255;

  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            // Increase red
          case SDLK_q:
            r += 2;
            break;

          // Increase green
          case SDLK_w:
            g += 2;
            break;

          // Increase blue
          case SDLK_e:
            b += 2;
            break;

          // Decrease red
          case SDLK_a:
            r -= 2;
            break;

          // Decrease green
          case SDLK_s:
            g -= 2;
            break;

          // Decrease blue
          case SDLK_d:
            b -= 2;
            break;
          default:
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    texture.SetColor(r, g, b);
    texture.Render(0, 0);

    SDL_RenderPresent(gRenderer);
  }
  Close();
  return 0;
}