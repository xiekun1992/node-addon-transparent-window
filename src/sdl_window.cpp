#include "sdl_window.h"

namespace sdl_window {
  BlankWindow::BlankWindow() {
    win = SDL_CreateWindow(
      "sdl_win",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      W,
      H,
      SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR
    );
  }
  BlankWindow::~BlankWindow() {}
  void BlankWindow::create(std::function<void(int, int)> const& update) {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    bool quit = false;

    while (!quit) {
      SDL_Event event;
      while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION) {
          update(event.motion.xrel, event.motion.yrel);
        }
      }
    }
  }
  void BlankWindow::close() {
    SDL_DestroyWindow(win);
    SDL_Quit();
  }
}