#ifndef _NODE_ADDON_SDL_WINDOW
#define _NODE_ADDON_SDL_WINDOW

#include <functional>

#include "SDL.h"

namespace sdl_window {
  class BlankWindow {
    private:
      int W = 800, H = 600;
      SDL_Window* win;
    public:
      BlankWindow();
      ~BlankWindow();
      void create(std::function<void(int, int)> const& update);
      void close();
  };
}

#endif