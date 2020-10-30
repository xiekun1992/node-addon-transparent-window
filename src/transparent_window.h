#pragma once

#ifndef _NODE_ADDON_TRANSPARENT_WINDOW
#define _NODE_ADDON_TRANSPARENT_WINDOW

#define UNICODE

#include <thread>
#include <chrono>
#include <functional>
#include <string>
#include <iostream>
#if __linux == 1

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>

#elif _WIN32 == 1

#include <shlobj.h>
#include <windows.h>

#endif

namespace transparent_window {
  class BlankWindow {
    private:
#if __linux == 1
      // Display* display;
      // Window win;
      int keep_running;
#elif _WIN32 == 1
      
#endif
    public:
      BlankWindow();
      ~BlankWindow();
      void create(std::function<void()> const& lambda);
      void close(int threadId);
  };
}

#endif