#include "core/Application.h"

// temporary function to draw the model hierarchy in ImGui
static void DrawModelNode(Model* model) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (model->m_children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;

    if (ImGui::TreeNodeEx((void*)model, flags, "%s", model->getName().c_str())) {
        ImGui::DragFloat3("Position", &model->m_position.x, 0.1f);

        for (auto* child : model->m_children) {
            DrawModelNode(child);
        }
        ImGui::TreePop();
    }
}

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

    auto test_model = std::make_unique<Model>("models/cat/12221_Cat_v1_l3.obj");

    m_scenes.push_back(std::make_unique<Scene>());
    m_activeScene = m_scenes.back().get();

    m_activeScene->addModel(std::move(test_model));

    auto bugatti2 = std::make_unique<Model>("models/bugatti/bugatti.obj");
    // bugatti2->m_position = glm::vec3(20.0f, 0.0f, 0.0f);
    // bugatti2->m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // bugatti2->m_scale    = glm::vec3(1.0f, 1.0f, 1.0f);
    m_activeScene->addModel(std::move(bugatti2));

    // Now get the raw pointers from the scene
    Model* catModel = m_activeScene->getModel(0);      // first model added
    Model* bugattiModel = m_activeScene->getModel(1);  // second model added

    if (bugattiModel && catModel) {
        bugattiModel->addChild(catModel);
    }

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

        update(deltaTime);

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
                // Only draw root models (those without a parent)
                if (!model->m_parent) {
                    DrawModelNode(model);
                }
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
    glm::mat4 projection = m_camera->getProjectionMatrix(1280.0f / 720.0f);

    Renderer::drawViewportGizmo(view, projection);
    Renderer::beginScene(view, projection); // this is where the skybox and stuff is 

    auto& models = m_activeScene->getModels();
    for (auto& model : models)
        Renderer::submit(*m_defaultShader, *model, view, projection);
}

void Application::update(float deltaTime) {
    Model* car = m_activeScene->getModel(0); // this is bugatti
    Model* cat = m_activeScene->getModel(1); // this is cat 

    if (car) {
        car->m_rotation.y += 20.0f * deltaTime; // rotate
    }

    if(cat) {
        cat->m_position = glm::vec3(25.0f, 0.0f,0.0f );
    }
}

void Application::stop() {
    m_isRunning = false;
}
