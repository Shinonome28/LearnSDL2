#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cassert>
#include <vector>

#include "common_utils.h"

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;
constexpr int kLevelWidth = 1280;
constexpr int kLevelHeight = 960;
constexpr int kHardwareMixerChannels = 2;
constexpr int kPlaybackFrequency = 44100;
constexpr int kMixerChunkSize = 2048;
constexpr int kDotWidth = 20;
constexpr int kDotHeight = 20;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;
SDL_Joystick* gJoyStick = nullptr;
SDL_Haptic* gJoyHaptic = nullptr;
SDL_GameController* gGameController = nullptr;

struct Circle {
  int x, y, r;
};

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
  void LoadFromFile(SDL_Renderer* renderer, const char* path);
  void LoadFromText(const char* text, SDL_Color color);

  int GetWidth() { return width_; }
  int GetHeight() { return height_; }

  void Render(int x, int y, SDL_Rect* clip = nullptr);
  void Render(int x, int y, TextureRenderOptions* options);

  void Render(SDL_Renderer* renderer, int x, int y,
              TextureRenderOptions* options);

  void SetColor(Uint8 Red, Uint8 Green, Uint8 Blue);
  void SetBlendeMode(SDL_BlendMode blend_mode);
  void SetAlpha(Uint8 alpha);
  bool IsLoaded() { return texture_ != nullptr; }
  void TryFree();

  ~UTexture() { Free(); }

 private:
  int width_;
  int height_;
  SDL_Texture* texture_ = nullptr;
};

UTexture gDotTexture;

void UTexture::TryFree() {
  if (IsLoaded()) Free();
}

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

void UTexture::LoadFromFile(SDL_Renderer* renderer, const char* path) {
  SDL_Surface* img = LoadMedia(path);
  // 使用默认的color key
  SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 0xFF, 0xFF));
  texture_ = SDL_CreateTextureFromSurface(renderer, img);
  check_error(texture_ == nullptr);
  width_ = img->w;
  height_ = img->h;
  SDL_FreeSurface(img);

  // 默认开启alpha混合
  SetBlendeMode(SDL_BLENDMODE_BLEND);
}

void UTexture::LoadFromFile(const char* path) { LoadFromFile(gRenderer, path); }
void UTexture::Render(int x, int y, SDL_Rect* clip) {
  TextureRenderOptions options;
  options.clip = clip;
  Render(gRenderer, x, y, &options);
}

void UTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(texture_, red, green, blue);
}

void UTexture::Render(int x, int y, TextureRenderOptions* options) {
  Render(gRenderer, x, y, options);
}

void UTexture::Render(SDL_Renderer* renderer, int x, int y,
                      TextureRenderOptions* options) {
  assert(renderer != nullptr);
  SDL_Rect render_rect = {x, y, width_, height_};
  if (options != nullptr && options->clip != nullptr) {
    render_rect.h = options->clip->h;
    render_rect.w = options->clip->w;
  }
  if (options != nullptr) {
    SDL_RenderCopyEx(renderer, texture_, options->clip, &render_rect,
                     options->angle, options->center, options->flip);
  } else {
    SDL_RenderCopy(renderer, texture_, nullptr, &render_rect);
  }
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

void Init(bool init_window = true) {
  check_error(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK |
                       SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

  if (init_window) {
    gWindow = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, kScreenWidth,
                               kScreenHeight, SDL_WINDOW_SHOWN);
    check_error(gWindow == nullptr);

    gRenderer = SDL_CreateRenderer(
        gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_error(gRenderer == nullptr);
    gDotTexture.LoadFromFile("images/dot.bmp");
  }

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
  if (gRenderer != nullptr) {
    SDL_DestroyRenderer(gRenderer);
  }
  if (gWindow != nullptr) {
    SDL_DestroyWindow(gWindow);
  }
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

class UDot {
 public:
  UDot(int x_vel_value, int y_vel_value)
      : x_vel_value_(x_vel_value), y_vel_value_(y_vel_value) {
    colliders_.resize(11);
    colliders_[0].w = 6;
    colliders_[0].h = 1;

    colliders_[1].w = 10;
    colliders_[1].h = 1;

    colliders_[2].w = 14;
    colliders_[2].h = 1;

    colliders_[3].w = 16;
    colliders_[3].h = 2;

    colliders_[4].w = 18;
    colliders_[4].h = 2;

    colliders_[5].w = 20;
    colliders_[5].h = 6;

    colliders_[6].w = 18;
    colliders_[6].h = 2;

    colliders_[7].w = 16;
    colliders_[7].h = 2;

    colliders_[8].w = 14;
    colliders_[8].h = 1;

    colliders_[9].w = 10;
    colliders_[9].h = 1;

    colliders_[10].w = 6;
    colliders_[10].h = 1;

    ShiftColliders_();
  }
  void HandleEvent(const SDL_Event& e, SDL_Keycode up_key, SDL_Keycode down_key,
                   SDL_Keycode left_key, SDL_Keycode right_key);
  void Move();
  void MoveInLevel();
  void Move(const SDL_Rect& wall);
  void Move(const std::vector<SDL_Rect>& other_colliders);
  void MoveInSceneUseCircleCollider(const SDL_Rect& wall, const Circle& other);
  void Render();
  void RenderRelativeToCamera(const SDL_Rect& camera);
  void ShiftTo(int x, int y) {
    x_pos_ = x;
    y_pos_ = y;
    ShiftColliders_();
  }

  Circle GetCircleCollider() { return circle_collider_; }

  int GetXPosition() { return x_pos_; }

  int GetYPosition() { return y_pos_; }

  std::vector<SDL_Rect> GetColliders() { return colliders_; };

 private:
  int x_pos_ = 0, y_pos_ = 0, x_vel_ = 0, y_vel_ = 0;
  const int x_vel_value_, y_vel_value_;
  std::vector<SDL_Rect> colliders_;
  Circle circle_collider_ = {0, 0, kDotWidth / 2};

  void ShiftColliders_();
};

void UDot::ShiftColliders_() {
  int r = 0;
  for (size_t i = 0; i < colliders_.size(); i++) {
    colliders_[i].x = x_pos_ + (kDotWidth - colliders_[i].w) / 2;
    colliders_[i].y = y_pos_ + r;
    r += colliders_[i].h;
  }
  circle_collider_.x = x_pos_ + circle_collider_.r;
  circle_collider_.y = y_pos_ + circle_collider_.r;
}

void UDot::HandleEvent(const SDL_Event& e, SDL_Keycode up_key,
                       SDL_Keycode down_key, SDL_Keycode left_key,
                       SDL_Keycode right_key) {
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    SDL_Keycode code = e.key.keysym.sym;
    if (code == up_key) {
      y_vel_ -= y_vel_value_;
    } else if (code == down_key) {
      y_vel_ += y_vel_value_;
    } else if (code == left_key) {
      x_vel_ -= x_vel_value_;
    } else if (code == right_key) {
      x_vel_ += x_vel_value_;
    }
  } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
    SDL_Keycode code = e.key.keysym.sym;
    if (code == up_key) {
      y_vel_ += y_vel_value_;
    } else if (code == down_key) {
      y_vel_ -= y_vel_value_;
    } else if (code == left_key) {
      x_vel_ += x_vel_value_;
    } else if (code == right_key) {
      x_vel_ -= x_vel_value_;
    }
  }
}

void UDot::Move() {
  x_pos_ += x_vel_;
  if ((x_pos_ < 0) || ((x_pos_ + kDotWidth) > kScreenWidth)) {
    x_pos_ -= x_vel_;
  }
  y_pos_ += y_vel_;
  if ((y_pos_) < 0 || ((y_pos_ + kDotHeight) > kScreenHeight)) {
    y_pos_ -= y_vel_;
  }
}

void UDot::MoveInLevel() {
  x_pos_ += x_vel_;
  if ((x_pos_ < 0) || ((x_pos_ + kDotWidth) > kLevelWidth)) {
    x_pos_ -= x_vel_;
  }
  y_pos_ += y_vel_;
  if ((y_pos_) < 0 || ((y_pos_ + kDotHeight) > kLevelHeight)) {
    y_pos_ -= y_vel_;
  }
}

void UDot::Render() { gDotTexture.Render(x_pos_, y_pos_); }

void UDot::RenderRelativeToCamera(const SDL_Rect& camera) {
  gDotTexture.Render(x_pos_ - camera.x, y_pos_ - camera.y);
}

bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
  const int left_a = a.x;
  const int right_a = a.x + a.w;
  const int top_a = a.y;
  const int bottom_a = a.y + a.h;
  const int left_b = b.x;
  const int right_b = b.x + b.w;
  const int top_b = b.y;
  const int bottom_b = b.y + b.h;

  if (right_a <= left_b || right_b <= left_a) {
    return false;
  }
  if (bottom_a <= top_b || bottom_b <= top_a) {
    return false;
  }

  return true;
}

bool CheckCollision(const std::vector<SDL_Rect>& a,
                    const std::vector<SDL_Rect>& b) {
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < b.size(); j++) {
      if (CheckCollision(a[i], b[j])) {
        return true;
      }
    }
  }
  return false;
}

void UDot::Move(const SDL_Rect& wall) {
  {
    x_pos_ += x_vel_;
    SDL_Rect collision_box = {x_pos_, y_pos_, kDotWidth, kDotHeight};
    if (x_pos_ < 0 || x_pos_ + kDotWidth > kScreenWidth ||
        CheckCollision(collision_box, wall)) {
      x_pos_ -= x_vel_;
    }
  }
  {
    y_pos_ += y_vel_;
    SDL_Rect collision_box = {x_pos_, y_pos_, kDotWidth, kDotHeight};
    if (y_pos_ < 0 || y_pos_ + kDotHeight > kScreenHeight ||
        CheckCollision(collision_box, wall)) {
      y_pos_ -= y_vel_;
    }
  }
}

void UDot::Move(const std::vector<SDL_Rect>& other_colliders) {
  x_pos_ += x_vel_;
  ShiftColliders_();
  if (x_pos_ < 0 || x_pos_ + kDotWidth > kScreenWidth ||
      CheckCollision(colliders_, other_colliders)) {
    x_pos_ -= x_vel_;
  }
  y_pos_ += y_vel_;
  ShiftColliders_();
  if (y_pos_ < 0 || y_pos_ + kDotHeight > kScreenHeight ||
      CheckCollision(colliders_, other_colliders)) {
    y_pos_ -= y_vel_;
  }
}

bool CheckCollision(const Circle& a, const Circle& b) {
  return square(a.x - b.x) + square(a.y - b.y) < square(a.r + b.r);
}

bool CheckCollision(const Circle& a, const SDL_Rect& b) {
  int c_x, c_y;
  if (a.x < b.x) {
    c_x = b.x;
  } else if (a.x > b.x + b.w) {
    c_x = b.x + b.w;
  }
  // in this case there must be a collision.
  // we do this to ensure the check to be success
  else {
    c_x = a.x;
  }

  if (a.y < b.y) {
    c_y = b.y;
  } else if (a.y > b.y + b.h) {
    c_y = b.y + b.h;
  } else {
    c_y = a.y;
  }

  return distanceSquared(a.x, a.y, c_x, c_y) < square(a.r);
}

void UDot::MoveInSceneUseCircleCollider(const SDL_Rect& wall,
                                        const Circle& other) {
  x_pos_ += x_vel_;
  ShiftColliders_();
  if (x_pos_ < 0 || x_pos_ + kDotWidth > kScreenWidth ||
      CheckCollision(circle_collider_, wall) ||
      CheckCollision(circle_collider_, other)) {
    x_pos_ -= x_vel_;
  }
  y_pos_ += y_vel_;
  ShiftColliders_();
  if (y_pos_ < 0 || y_pos_ + kDotHeight > kScreenHeight ||
      CheckCollision(circle_collider_, wall) ||
      CheckCollision(circle_collider_, other)) {
    y_pos_ -= y_vel_;
  }
}

class UWindow {
 public:
  void Init();
  SDL_Renderer* CreateRenderer();
  void HandleEvent(const SDL_Event& e);
  void TryFree() noexcept;

  ~UWindow() { TryFree(); }

  int GetWidth();
  int GetHeight();

  bool HasMouseFocus() {
    assert(window_ != nullptr);
    return has_mouse_focus_;
  };
  bool HasKeyboardFocus() {
    assert(window_ != nullptr);
    return has_keyboard_focus_;
  }
  bool IsMinimized() {
    assert(window_ != nullptr);
    return is_minimized_;
  }
  bool IsFullScreen() { return is_full_screen_; }
  SDL_Renderer* GetRenderer() { return renderer_; }

 private:
  SDL_Window* window_ = nullptr;
  int height_, width_;
  bool has_mouse_focus_, has_keyboard_focus_, is_minimized_, is_full_screen_;
  SDL_Renderer* renderer_ = nullptr;
};

void UWindow::Init() {
  window_ = SDL_CreateWindow(
      "SDL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      kScreenWidth, kScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  check_error(window_ == nullptr);
  height_ = kScreenHeight;
  width_ = kScreenWidth;
  has_mouse_focus_ = true;
  has_keyboard_focus_ = true;
  is_minimized_ = false;
  is_full_screen_ = false;
}

void UWindow::TryFree() noexcept {
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  if (renderer_ != nullptr) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }
}

SDL_Renderer* UWindow::CreateRenderer() {
  assert(window_ != nullptr);
  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  check_error(renderer_ == nullptr);
  return renderer_;
}

void UWindow::HandleEvent(const SDL_Event& e) {
  assert(renderer_ != nullptr);
  if (e.type == SDL_WINDOWEVENT) {
    bool update_caption = false;
    switch (e.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        width_ = e.window.data1;
        height_ = e.window.data2;
        SDL_RenderPresent(renderer_);
        break;
      case SDL_WINDOWEVENT_EXPOSED:
        SDL_RenderPresent(renderer_);
        break;
      case SDL_WINDOWEVENT_ENTER:
        has_mouse_focus_ = true;
        update_caption = true;
        break;
      case SDL_WINDOWEVENT_LEAVE:
        has_mouse_focus_ = false;
        update_caption = true;
        break;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        has_keyboard_focus_ = true;
        update_caption = true;
        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        has_keyboard_focus_ = false;
        update_caption = true;
        break;
      case SDL_WINDOWEVENT_MINIMIZED:
        is_minimized_ = true;
        break;
      case SDL_WINDOWEVENT_MAXIMIZED:
        is_minimized_ = false;
        break;
      default:
        break;
    }
    if (update_caption) {
      std::stringstream caption;
      caption << "SDL Tutorial - MouseFocus:"
              << ((has_mouse_focus_) ? "On" : "Off")
              << " KeyboardFocus:" << ((has_keyboard_focus_) ? "On" : "Off");
      SDL_SetWindowTitle(window_, caption.str().c_str());
    }
  } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
    SDL_SetWindowFullscreen(window_, is_full_screen_ ? SDL_FALSE : SDL_TRUE);
    is_full_screen_ = !is_full_screen_;
  }
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

int ExampleMain2(int argc, char** argv) {
  Init(false);

  UWindow window;
  window.Init();
  SDL_Renderer* renderer = window.CreateRenderer();

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      window.HandleEvent(e);
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  window.TryFree();
  Close();

  return 0;
}