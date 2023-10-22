#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "common_utils.h"

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 680;
constexpr int kHardwareMixerChannels = 2;
constexpr int kPlaybackFrequency = 44100;
constexpr int kMixerChunkSize = 2048;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;
SDL_Joystick* gJoyStick = nullptr;
SDL_Haptic* gJoyHaptic = nullptr;
SDL_GameController* gGameController = nullptr;

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

enum UButtonState {
  BUTTON_MOUSE_OUT,
  BUTTON_MOUSE_OVER_MOTION,
  BUTTON_MOUSE_DOWN,
  BUTTON_MOUSE_UP,
  BUTTON_MOUSE_UNKNOWN
};

class UButton {
 public:
  UButton(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}
  void HandleEvent(SDL_Event* e);
  UButtonState GetState() { return state_; };
  int GetX() { return x_; }
  int GetY() { return y_; }

 private:
  UButtonState state_ = BUTTON_MOUSE_UNKNOWN;
  int x_, y_, w_, h_;
};

void UButton::HandleEvent(SDL_Event* e) {
  if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||
      e->type == SDL_MOUSEBUTTONUP) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool inside = true;

    if (x < x_) {
      inside = false;
    } else if (x > x_ + w_) {
      inside = false;
    } else if (y < y_) {
      inside = false;
    } else if (y > y_ + h_) {
      inside = false;
    }

    if (!inside) {
      state_ = BUTTON_MOUSE_OUT;
      return;
    }

    switch (e->type) {
      case SDL_MOUSEMOTION:
        state_ = BUTTON_MOUSE_OVER_MOTION;
        break;

      case SDL_MOUSEBUTTONDOWN:
        state_ = BUTTON_MOUSE_DOWN;
        break;

      case SDL_MOUSEBUTTONUP:
        state_ = BUTTON_MOUSE_UP;
        break;
    }
  }
}

Mix_Music* LoadMusic(const char* path) {
  Mix_Music* r = Mix_LoadMUS(path);
  check_error_mixer(r == nullptr);
  return r;
}

Mix_Chunk* LoadChunk(const char* path) {
  Mix_Chunk* r = Mix_LoadWAV(path);
  check_error_mixer(r == nullptr);
  return r;
}

void Init() {
  check_error(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK |
                       SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
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

  check_error_mixer(Mix_OpenAudio(kPlaybackFrequency, MIX_DEFAULT_FORMAT,
                                  kHardwareMixerChannels, kMixerChunkSize) < 0);
}

void InitGameController() {
  ensure_custom(SDL_NumJoysticks() >= 1, "No Game Controller.");
  if (!SDL_IsGameController(0)) {
    gJoyStick = SDL_JoystickOpen(0);
    ensure(gJoyStick != nullptr);
    if (SDL_JoystickIsHaptic(gJoyStick)) {
      gJoyHaptic = SDL_HapticOpenFromJoystick(gJoyStick);
      if (gJoyHaptic != nullptr) {
        SDL_HapticRumbleInit(gJoyHaptic);
      }
    }
  } else {
    gGameController = SDL_GameControllerOpen(0);
    ensure(gGameController != nullptr);
  }
}

void Close() {
  if (gGameController != nullptr) {
    SDL_GameControllerClose(gGameController);
  }
  if (gJoyHaptic != nullptr) {
    SDL_HapticClose(gJoyHaptic);
  }
  if (gJoyStick != nullptr) {
    SDL_JoystickClose(gJoyStick);
  }
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  IMG_Quit();
  SDL_Quit();
}

class UTimer {
 public:
  void Start();
  void Stop();
  void Pause();
  void Unpause();

  Uint32 GetTime();

  bool IsStarted() { return is_started_; };
  bool IsPaused() { return is_paused_; };
  bool CanStart() { return !is_started_; }
  bool CanStop() { return is_started_; }
  bool CanPause() { return is_started_ && !is_paused_; }
  bool CanUnpause() { return is_started_ && is_paused_; }

 private:
  Uint32 started_time_;
  Uint32 paused_time_;
  bool is_started_ = false;
  bool is_paused_ = false;
};

void UTimer::Start() {
  started_time_ = SDL_GetTicks();
  is_started_ = true;
}

void UTimer::Stop() {
  is_started_ = false;
  is_paused_ = false;
}

void UTimer::Pause() {
  paused_time_ = SDL_GetTicks();
  is_paused_ = true;
}

Uint32 UTimer::GetTime() {
  if (is_paused_) {
    return paused_time_ - started_time_;
  } else if (!is_started_) {
    return 0;
  } else {
    return SDL_GetTicks() - started_time_;
  }
}

void UTimer::Unpause() {
  started_time_ = SDL_GetTicks() - (paused_time_ - started_time_);
  is_paused_ = false;
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