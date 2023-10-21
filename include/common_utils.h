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

 public:
  struct StreamEndT {};
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
DebugOutputStream::StreamEndT &debug_end() {
  static DebugOutputStream::StreamEndT v;
  return v;
}

void check_error(bool flag) {
  if (flag) {
    output__(SDL_GetError());
    exit(1);
  }
}

void check_error_custom(bool flag, const char *msg) {
  if (flag) {
    output__(msg);
    exit(1);
  }
}

void check_error_img(bool flag) {
  if (flag) {
    output__(IMG_GetError());
    exit(1);
  }
}

void check_error_ttf(bool flag) {
  if (flag) {
    output__(TTF_GetError());
    exit(1);
  }
}

void ensure(bool flag) {
  if (!flag) {
    output__(SDL_GetError());
    exit(1);
  }
}

void ensure_custom(bool flag, const char *msg) {
  if (!flag) {
    output__(msg);
    exit(1);
  }
}

void ensure_img(bool flag) {
  if (!flag) {
    output__(IMG_GetError());
    exit(1);
  }
}

void ensure_ttf(bool flag) {
  if (!flag) {
    output__(TTF_GetError());
    exit(1);
  }
}