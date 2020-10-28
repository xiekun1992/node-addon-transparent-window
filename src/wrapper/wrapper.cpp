#include <napi.h>
#include "transparent_window_wrapper.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  TransparentWindow::initMethods(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll);