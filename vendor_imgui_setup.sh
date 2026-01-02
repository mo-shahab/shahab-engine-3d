#!/bin/bash

mkdir -p ./vendor/imgui/backends

wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui.h -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imconfig.h -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_draw.cpp -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_widgets.cpp -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_tables.cpp -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_internal.h -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imstb_rectpack.h -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imstb_textedit.h -P vendor/imgui/
wget https://raw.githubusercontent.com/ocornut/imgui/master/imstb_truetype.h -P vendor/imgui/

wget https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_glfw.cpp -P vendor/imgui/backends
wget https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_glfw.h -P vendor/imgui/backends
wget https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_opengl3.cpp -P vendor/imgui/backends
wget https://raw.githubusercontent.com/ocornut/imgui/master/backends/imgui_impl_opengl3.h -P vendor/imgui/backends
