#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "common_utils.h"

struct TextureRenderOptions {
  SDL_Rect* clip = nullptr;
  double angle = 0.0;
  SDL_Point* center = nullptr;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
};

class UTexture {
 public:
  void Free();
  void LoadFromFile(const char* path);
  void LoadFromText(const char* text, SDL_Color color);

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

  void Render(int x, int y, const SDL_Rect* clip = nullptr);
  void Render(int x, int y, TextureRenderOptions* options);
  void SetColor(Uint8 Red, Uint8 Green, Uint8 Blue);
  void SetBlendeMode(SDL_BlendMode blend_mode);
  void SetAlpha(Uint8 alpha);

  ~UTexture() { Free(); }

 private:
  int width_;
  int height_;
  SDL_Texture* texture_ = nullptr;
};

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

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
  // 使用默认的color key
  SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 0xFF, 0xFF));
  texture_ = SDL_CreateTextureFromSurface(gRenderer, img);
  check_error(texture_ == nullptr);
  width_ = img->w;
  height_ = img->h;
  SDL_FreeSurface(img);

  // 默认开启alpha混合
  SetBlendeMode(SDL_BLENDMODE_BLEND);
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

void UTexture::Render(int x, int y, TextureRenderOptions* options) {
  SDL_Rect render_rect = {x, y, width_, height_};
  if (options->clip != nullptr) {
    render_rect.h = options->clip->h;
    render_rect.w = options->clip->w;
  }
  SDL_RenderCopyEx(gRenderer, texture_, options->clip, &render_rect,
                   options->angle, options->center, options->flip);
}

void UTexture::SetBlendeMode(SDL_BlendMode blend_mode) {
  SDL_SetTextureBlendMode(texture_, blend_mode);
}

void UTexture::SetAlpha(Uint8 alpha) {
  SDL_SetTextureAlphaMod(texture_, alpha);
}

void UTexture::LoadFromText(const char* text, SDL_Color color) {
  SDL_Surface* rendered_text_surface = TTF_RenderText_Solid(gFont, text, color);
  check_error(rendered_text_surface == nullptr);
  texture_ = SDL_CreateTextureFromSurface(gRenderer, rendered_text_surface);
  check_error(texture_ == nullptr);
  width_ = rendered_text_surface->w;
  height_ = rendered_text_surface->h;
  SDL_FreeSurface(rendered_text_surface);
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO) < 0);
  gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                             kScreenHeight, SDL_WINDOW_SHOWN);
  check_error(gWindow == nullptr);

  gRenderer = SDL_CreateRenderer(
      gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  check_error(gRenderer == nullptr);

  ensure_img(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
  ensure_ttf(TTF_Init() == 0);

  gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
  check_error_ttf(gFont == nullptr);
}

void Close() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  IMG_Quit();
  SDL_Quit();
}

// example main function
int ExampleMain1(int argc, char** argv) {
  Init();

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

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}