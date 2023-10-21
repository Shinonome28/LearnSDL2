#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <Windows.h>

#include <sstream>

void output__(const char *str) {
  MessageBox(nullptr, str, "Error!", MB_OK | MB_ICONERROR);
}

class DebugOutputStream {
  friend DebugOutputStream debug();
  struct StreamEndT {};

 public:
  template <typename T>
  DebugOutputStream &operator<<(const T &value) {
    (*cache) << value;
    return *this;
  }

  template <>
  DebugOutputStream &operator<<(const StreamEndT &) {
    output__(cache->str().c_str());
    delete cache;
    cache = nullptr;
    return *this;
  }

  static StreamEndT End() { return StreamEndT(); }

  ~DebugOutputStream() {
    if (cache != nullptr) {
      delete cache;
    }
  }

 private:
  DebugOutputStream() { cache = new std::stringstream(); }

  std::stringstream *cache = nullptr;
};

DebugOutputStream debug() { return DebugOutputStream(); }

inline void check_error(bool flag) {
  if (flag) {
    output__(SDL_GetError());
    exit(1);
  }
}

inline void check_error_img(bool flag) {
  if (flag) {
    output__(IMG_GetError());
    exit(1);
  }
}

inline void check_error_ttf(bool flag) {
  if (flag) {
    output__(TTF_GetError());
    exit(1);
  }
}

inline void ensure(bool flag) {
  if (!flag) {
    output__(SDL_GetError());
    exit(1);
  }
}

inline void ensure_img(bool flag) {
  if (!flag) {
    output__(IMG_GetError());
    exit(1);
  }
}

inline void ensure_ttf(bool flag) {
  if (!flag) {
    output__(TTF_GetError());
    exit(1);
  }
}