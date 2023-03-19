#pragma once

#ifndef _TRANSPARENT_WINDOW_WRAPPER
#define _TRANSPARENT_WINDOW_WRAPPER

#include <napi.h>
// #include "../transparent_window.h"
#include "../sdl_window.h"

namespace TransparentWindow {
  Napi::Value create(const Napi::CallbackInfo& info);
  Napi::Value close(const Napi::CallbackInfo& info);
  // Napi::Value topmost(const Napi::CallbackInfo& info);
  Napi::Object initMethods(Napi::Env env, Napi::Object exports);
}

#endif