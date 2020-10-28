#pragma once

#ifndef _NODE_ADDON_TRANSPARENT_WINDOW
#define _NODE_ADDON_TRANSPARENT_WINDOW

#if __linux == 1

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <thread>
#include <chrono>

#elif _WIN32 == 1
#endif

namespace transparent_window {
  class Window {
    private:
#if __linux == 1
      Display* display;
      Window win;
      int keep_running;
#endif
    public:
      Window();
      ~Window();
      void create();
      void close();
  };
}

#endif