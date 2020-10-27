{
  "targets": [
    {
      "target_name": "transparent_window",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": [
        "src/transparent_window.cpp",
        "src/wrapper/transparent_window_wrapper.cpp",
        "src/wrapper/wrapper.cpp"
      ],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "conditions": [
        {
          "OS == 'linux'", {
            "link_settings": {
              "libraries": ["-lX11"]
            }
          }
        }
      ]
    }
  ]
}