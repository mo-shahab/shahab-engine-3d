#include "core/Application.h"

Application::Application(const std::string& title)
    : m_mainWindow(std::make_unique<Window>(1280, 720, title))
    , m_camera(std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 45.0f)))
    , m_isRunning(false) {

    glfwMakeContextCurrent(m_mainWindow->getNativeWindow());

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    Renderer::init(); // static method to initialize the renderer
    Input::init(m_mainWindow->getNativeWindow()); // static method to initialize input system

    m_defaultShader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");

    auto bugatti = std::make_unique<Model>("bugatti/bugatti.obj");
    m_models.push_back(std::move(bugatti));

    m_projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
}

Application::~Application() {}

void Application::run() {
    m_isRunning = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (m_isRunning && !m_mainWindow->shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // all the camera movement handling

        // keyboard input
        if (Input::isKeyPressed(GLFW_KEY_W)) m_camera->processKeyboard(FORWARD, deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_S)) m_camera->processKeyboard(BACKWARD, deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_A)) m_camera->processKeyboard(LEFT, deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_D)) m_camera->processKeyboard(RIGHT, deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_Q)) m_camera->processKeyboard(UP, deltaTime);
        if (Input::isKeyPressed(GLFW_KEY_E)) m_camera->processKeyboard(DOWN, deltaTime);


        // mouse movement
        if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            static bool firstMouse = true;
            static float lastX = 640.0f; // center of the window
            static float lastY = 360.0f; // center of the window

            glm::vec2 mousePos = Input::getMousePosition();
            float xPos = mousePos.x;
            float yPos = mousePos.y;

            if (firstMouse) {
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
            }

            float xoffset = xPos - lastX;
            float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

            lastX = xPos;
            lastY = yPos;

            m_camera->processMouseMovement(xoffset, yoffset);
        }

        render();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_mainWindow->swapBuffers();
        m_mainWindow->pollEvents();
    }
}

void Application::render() {
    Renderer::clear(0.1f, 0.1f, 0.1f, 1.0f);

    glm::mat4 view = m_camera->getViewMatrix(); 

    for (auto& model : m_models)
        Renderer::submit(*m_defaultShader, *model, view, m_projectionMatrix);
}

void Application::stop() {
    m_isRunning = false;
}
