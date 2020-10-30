#include "transparent_window_wrapper.h"

transparent_window::BlankWindow* windowPtr;
std::thread thread;
Napi::ThreadSafeFunction tsfn;
int threadId = 0;
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
      threadId = GetCurrentThreadId();
      auto callback = [](Napi::Env env, Napi::Function jsCallback) {
        jsCallback.Call({});
      };
      transparent_window::BlankWindow window;
      windowPtr = &window;
      auto createHandler = [=]() {
        tsfn.BlockingCall(callback);
      };
      window.create(createHandler);
    });
    return info.Env().Undefined();
  }
  Napi::Value close(const Napi::CallbackInfo& info) {
    windowPtr->close(threadId);
    tsfn.Release();
    // delete window;
    return info.Env().Undefined();
  }
  Napi::Object initMethods(Napi::Env env, Napi::Object exports) {
    exports.Set("create", Napi::Function::New(env, TransparentWindow::create));
    exports.Set("close", Napi::Function::New(env, TransparentWindow::close));
    return exports;
  }
}