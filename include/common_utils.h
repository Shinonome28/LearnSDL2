#pragma once

#include <SDL2/SDL.h>

inline void check_error(bool flag) {
  if (flag) {
    puts(SDL_GetError());
    exit(1);
  }
}