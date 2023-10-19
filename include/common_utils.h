#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

inline void check_error(bool flag) {
  if (flag) {
    puts(SDL_GetError());
    exit(1);
  }
}

inline void check_error_img(bool flag) {
  if (flag) {
    puts(IMG_GetError());
    exit(1);
  }
}