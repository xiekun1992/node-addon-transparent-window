#include "transparent_window_wrapper.h"

sdl_window::BlankWindow* windowPtr;
// transparent_window::BlankWindow* windowPtr;
std::thread thread;
Napi::ThreadSafeFunction tsfn;
// int threadId = 0;
namespace TransparentWindow {
  Napi::Value create(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
      throw Napi::Error::New(env, "param callback required");
    } 
    if (!info[0].IsFunction()) {
      throw Napi::TypeError::New(env, "expect callback to be a function");
    }

    tsfn = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "tsfn", 0, 1, [](Napi::Env) {
      thread.join();
    });
    thread = std::thread([]() {
#if _WIN32 == 1
      // threadId = GetCurrentThreadId();
#endif
      auto callback = [](Napi::Env env, Napi::Function jsCallback, int* pos) {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set(Napi::String::New(env, "x"), pos[0]);
        obj.Set(Napi::String::New(env, "y"), pos[1]);
        jsCallback.Call({obj});
      };
      sdl_window::BlankWindow window;
      // transparent_window::BlankWindow window;
      windowPtr = &window;
      auto createHandler = [=](int x, int y) {
        int pos[2] = {x, y};
        printf("x %d, y %d\n", x, y);
        tsfn.BlockingCall(pos, callback);
      };
      window.create(createHandler);
    });
    return info.Env().Undefined();
  }
  Napi::Value close(const Napi::CallbackInfo& info) {
    windowPtr->close();
    // windowPtr->close(threadId);
    tsfn.Release();
    // delete window;
    return info.Env().Undefined();
  }
  // Napi::Value topmost(const Napi::CallbackInfo& info) {
  //   windowPtr->topmost();
  //   return info.Env().Undefined();
  // }
  Napi::Object initMethods(Napi::Env env, Napi::Object exports) {
    // exports.Set("topmost", Napi::Function::New(env, TransparentWindow::topmost));
    exports.Set("create", Napi::Function::New(env, TransparentWindow::create));
    exports.Set("close", Napi::Function::New(env, TransparentWindow::close));
    return exports;
  }
}