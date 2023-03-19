{
  "targets": [
    {
      "target_name": "transparent_window",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": [
        # "src/transparent_window.cpp",
        "src/sdl_window.cpp",
        "src/wrapper/transparent_window_wrapper.cpp",
        "src/wrapper/wrapper.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<(module_root_dir)/include/"
      ],
      "copies": [
        {
          "destination": "<(module_root_dir)/build/Release/",
          "files": [
            "<(module_root_dir)/lib/x64/SDL2.dll"
          ]
        }
      ],
      "link_settings": {
        "libraries": [
          "<(module_root_dir)/lib/x64/SDL2.lib"
        ]
      },
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "conditions": [
        [
          "OS == 'linux'", {
            "link_settings": {
              # "libraries": ["-lX11", "-lXfixes"]
              "libraries": ["-lSDL"]
            }
          }
        ]
      ]
    }
  ]
}