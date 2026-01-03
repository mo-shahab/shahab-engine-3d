#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

// Include necessary ImGui and GLFW headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// static class for handling ImGui operations
class ImguiLayer {
public:
    static void init(GLFWwindow* window);
    static void begin();
    static void shutdown();
    static void end();
};

#endif // IMGUI_LAYER_H