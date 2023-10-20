#include <SDL2/SDL.h>

#include "common_utils.h"

class UTexture {
 public:
  void Free();
  void LoadFromFile(const char* path);

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

  void Render(int x, int y);

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
UTexture gFooTexture;
UTexture gBackgroundTexture;

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

void UTexture::Render(int x, int y) {
  SDL_Rect render_rect = {x, y, width_, height_};
  SDL_RenderCopy(gRenderer, texture_, nullptr, &render_rect);
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  check_error(gRenderer == nullptr);
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  ensure_img(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
  gFooTexture.LoadFromFile("images/foo.png");
  gBackgroundTexture.LoadFromFile("images/background.png");
}

void Close() {
  gFooTexture.Free();
  gBackgroundTexture.Free();
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char** argv) {
  Init();

  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    gBackgroundTexture.Render(0, 0);
    gFooTexture.Render(240, 190);
    SDL_RenderPresent(gRenderer);
  }
  Close();
  return 0;
}