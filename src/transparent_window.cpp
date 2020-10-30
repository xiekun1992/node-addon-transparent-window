#include "transparent_window.h"

#if _WIN32 == 1
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
    case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
#endif

namespace transparent_window {
  BlankWindow::BlankWindow() {
#if __linux == 1
    keep_running = 1;
#endif
  }
  BlankWindow::~BlankWindow() {}
  void BlankWindow::create(std::function<void()> const& lambda) {
#if __linux == 1
    keep_running = 1;
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
#elif _WIN32 == 1
    const wchar_t CLASS_NAME[] = L"Transparent Window Class";

    HINSTANCE hInstance = GetModuleHandle(0);
    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
      0,                              // Optional window styles.
      CLASS_NAME,                     // Window class
      L"Transparent Window",    // Window text
      WS_OVERLAPPED,            // Window style
      // Size and position
      -10, -10, GetSystemMetrics(SM_CXSCREEN) + 20, GetSystemMetrics(SM_CYSCREEN) + 20,
      NULL,       // Parent window    
      NULL,       // Menu
      hInstance,  // Instance handle
      NULL        // Additional application data
    );
    if (hwnd != NULL) {
      SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & (~(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX)));
      SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & (~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME)) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
      SetLayeredWindowAttributes(hwnd, 0, 1, LWA_ALPHA | LWA_COLORKEY);
      // SetWindowPos(hwnd, HWND_TOPMOST, -10, - 10, GetSystemMetrics(SM_CXSCREEN) + 20, GetSystemMetrics(SM_CYSCREEN) + 20, SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_DRAWFRAME);
      ShowCursor(false);
      
      ShowWindow(hwnd, SW_SHOWDEFAULT);
      UpdateWindow(hwnd);
      
      lambda();

      MSG msg;
      while (GetMessage(&msg, nullptr, 0, 0))
      {
        if (msg.message == WM_QUIT) {
          break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
#endif
  }
  void BlankWindow::close(int threadId) {
#if __linux == 1
    keep_running = 0;
#elif _WIN32 == 1
    PostThreadMessage(threadId, WM_QUIT, 0, 0);
    // DestroyWindow(hwnd);
#endif
  }
}
