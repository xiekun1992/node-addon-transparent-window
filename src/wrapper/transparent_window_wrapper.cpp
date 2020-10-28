#include "transparent_window_wrapper.h"

transparent_window::Window window;
namespace TransparentWindow {
  Napi::Value create(const Napi::CallbackInfo& info) {
    window.create();
    return info.Env().Undefined();
  }
  Napi::Value close(const Napi::CallbackInfo& info) {
    window.close();
    return info.Env().Undefined();
  }
  Napi::Object initMethods(Napi::Env env, Napi::Object exports) {
    exports.Set("create", Napi::Function::New(env, Clipboard::create));
    exports.Set("close", Napi::Function::New(env, Clipboard::close));
    return exports;
  }
}