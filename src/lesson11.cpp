#include <SDL2/SDL.h>

#include "common_utils.h"

class UTexture {
 public:
  void Free();
  void LoadFromFile(const char* path);

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

  void Render(int x, int y, const SDL_Rect* clip = nullptr);

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
SDL_Rect gSpriteClips[4];
UTexture gSpriteSheetTexture;

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

  gSpriteSheetTexture.LoadFromFile("images/dots.png");

  gSpriteClips[0].x = 0;
  gSpriteClips[0].y = 0;
  gSpriteClips[0].w = 100;
  gSpriteClips[0].h = 100;

  // Set top right sprite
  gSpriteClips[1].x = 100;
  gSpriteClips[1].y = 0;
  gSpriteClips[1].w = 100;
  gSpriteClips[1].h = 100;

  // Set bottom left sprite
  gSpriteClips[2].x = 0;
  gSpriteClips[2].y = 100;
  gSpriteClips[2].w = 100;
  gSpriteClips[2].h = 100;

  // Set bottom right sprite
  gSpriteClips[3].x = 100;
  gSpriteClips[3].y = 100;
  gSpriteClips[3].w = 100;
  gSpriteClips[3].h = 100;
}

void Close() {
  gSpriteSheetTexture.Free();
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

    // Render top left sprite
    gSpriteSheetTexture.Render(0, 0, &gSpriteClips[0]);

    // Render top right sprite
    gSpriteSheetTexture.Render(kScreenWidth - gSpriteClips[1].w, 0,
                               &gSpriteClips[1]);

    // Render bottom left sprite
    gSpriteSheetTexture.Render(0, kScreenHeight - gSpriteClips[2].h,
                               &gSpriteClips[2]);

    // Render bottom right sprite
    gSpriteSheetTexture.Render(kScreenWidth - gSpriteClips[3].w,
                               kScreenHeight - gSpriteClips[3].h,
                               &gSpriteClips[3]);

    SDL_RenderPresent(gRenderer);
  }
  Close();
  return 0;
}