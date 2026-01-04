#include "core/Application.h"

Application::Application(const std::string& title)
    : m_mainWindow(std::make_unique<Window>(1280, 720, title))
    , m_camera(std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 45.0f)))
    , m_isRunning(false) 
{

    glfwMakeContextCurrent(m_mainWindow->getNativeWindow());

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    Renderer::init(); // static method to initialize the renderer
    Input::init(m_mainWindow->getNativeWindow()); // static method to initialize input system
    ImguiLayer::init(m_mainWindow->getNativeWindow()); // initialize ImGui layer

    m_defaultShader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");

    auto test_model = std::make_unique<Model>("models/cottage_fbx.fbx");

    m_scenes.push_back(std::make_unique<Scene>());
    m_activeScene = m_scenes.back().get(); // will set the last added scene as active scene

    m_activeScene->addModel(std::move(test_model));

    // auto bugatti2 = std::make_unique<Model>("models/bugatti/bugatti.obj");
    // bugatti2->m_position = glm::vec3(20.0f, 0.0f, 0.0f);
    // bugatti2->m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // bugatti2->m_scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    // m_activeScene->addModel(std::move(bugatti2));


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

        // 3d rendering 
        render();

        ImguiLayer::begin();

        ImGui::Begin("Engine Control Center");

        // --- 1. PERFORMANCE SECTION ---
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "PERFORMANCE");
        ImGui::Separator();

        // Using a ternary check to prevent division by zero on frame 1
        float fps = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0f);

        ImGui::Spacing();
        ImGui::Spacing();

        // --- 2. SCENE OUTLINER SECTION ---
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "SCENE OUTLINER");
        ImGui::Separator();

        auto& models = m_activeScene->getModels();

        if (models.empty()) {
            ImGui::Text("Scene is empty. Load a model to begin.");
        } else {
            for (size_t i = 0; i < models.size(); i++) {
                Model* model = models[i].get();
                
                // PushID is critical here to keep sliders independent!
                ImGui::PushID(static_cast<int>(i));

                std::string label = "Object: " + model->getName();
                
                if (ImGui::CollapsingHeader(label.c_str())) {
                    ImGui::Indent();
                    
                    ImGui::DragFloat3("Position", &model->m_position.x, 0.1f);
                    ImGui::DragFloat3("Rotation", &model->m_rotation.x, 1.0f);
                    ImGui::DragFloat3("Scale",    &model->m_scale.x,    0.01f);
                    
                    ImGui::Unindent();
                }

                ImGui::PopID();
                ImGui::Spacing();
            }
        }

        ImGui::End();

        ImguiLayer::end();


        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_mainWindow->swapBuffers();
        m_mainWindow->pollEvents();
    }
}

void Application::render() {
    Renderer::clear(0.1f, 0.1f, 0.1f, 1.0f);

    glm::mat4 view = m_camera->getViewMatrix(); 

    Renderer::drawViewportGizmo(view, m_projectionMatrix);

    auto& models = m_activeScene->getModels();
    for (auto& model : models)
        Renderer::submit(*m_defaultShader, *model, view, m_projectionMatrix);
}

void Application::stop() {
    m_isRunning = false;
}
