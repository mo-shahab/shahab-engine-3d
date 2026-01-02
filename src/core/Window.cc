#include "core/Window.h"

Window::Window(int width, int height, const std::string& title) {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if(!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
}

// destroy the window object and terminate glfw
Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

GLFWwindow* Window::getNativeWindow() {
    return m_window;
}