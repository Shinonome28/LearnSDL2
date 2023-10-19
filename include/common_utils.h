#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Windows.h>

inline void output__(const char *str) {
  MessageBox(nullptr, str, "Error!", MB_OK | MB_ICONERROR);
}

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