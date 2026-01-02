#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    GLFWwindow* getNativeWindow();

private:
    GLFWwindow* m_window;
};

#endif // WINDOW_H