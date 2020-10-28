#include "transparent_window.h"

namespace transparent_window {
  BlankWindow::BlankWindow() {
#if __linux == 1
    keep_running = 1;
#endif
  }
  void BlankWindow::create(std::function<void()> const& lambda) {
#if __linux == 1
    Display* display = XOpenDisplay(NULL);

    XVisualInfo vinfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;

    Window win = XCreateWindow(display, DefaultRootWindow(display), 0, 0, 3000, 2000, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);
    XSelectInput(display, win, StructureNotifyMask);
    GC gc = XCreateGC(display, win, 0, 0);

    // Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    // XSetWMProtocols(display, win, &wm_delete_window, 1);

    Atom window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    long value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(display, win, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char *) &value, 1);

    XFixesHideCursor(display, win);
    XFlush(display);

    XMapWindow(display, win);

    XEvent event;
    lambda();
    while (keep_running) {
      if (XPending(display)) {
        XNextEvent(display, &event);
      }

      // switch(event.type) {
      //     case ClientMessage:
      //         if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", 1) && (Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", 1))
      //             keep_running = 0;

      //         break;

      //     default:
      //         break;
      // }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    XFixesShowCursor(display, win);
    XFlush(display);

    XDestroyWindow(display, win);
    XCloseDisplay(display);
#endif
  }
  void BlankWindow::close() {
#if __linux == 1
    keep_running = 0;
#endif
  }
  BlankWindow::~BlankWindow() {
#if __linux == 1
#endif
  }
}
