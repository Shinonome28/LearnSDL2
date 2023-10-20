#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Windows.h>

#include <sstream>

void output__(const char *str) {
  MessageBox(nullptr, str, "Error!", MB_OK | MB_ICONERROR);
}

class DebugStream {
 public:
  template <typename T>
  DebugStream &operator<<(const T &value) {
    std::stringstream ss;
    ss << value;
    output__(ss.str().c_str());
    return *this;
  }
};

DebugStream gDebugStream;

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