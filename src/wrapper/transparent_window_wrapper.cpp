#include "transparent_window_wrapper.h"

transparent_window::BlankWindow window;
std::thread thread;
Napi::ThreadSafeFunction tsfn;
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
      auto callback = [](Napi::Env env, Napi::Function jsCallback) {
        jsCallback.Call({});
      };
      auto createHandler = [=]() {
        tsfn.BlockingCall(callback);
      };
      window.create(createHandler);
    });
    return info.Env().Undefined();
  }
  Napi::Value close(const Napi::CallbackInfo& info) {
    window.close();
    tsfn.Release();
    return info.Env().Undefined();
  }
  Napi::Object initMethods(Napi::Env env, Napi::Object exports) {
    exports.Set("create", Napi::Function::New(env, TransparentWindow::create));
    exports.Set("close", Napi::Function::New(env, TransparentWindow::close));
    return exports;
  }
}