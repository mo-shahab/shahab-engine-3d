#include "renderer/Renderer.h"

// static member definitions
GLuint Renderer::s_LineVAO = 0;
GLuint Renderer::s_LineVBO = 0;

std::unique_ptr<Shader> Renderer::s_lineShader = nullptr;
std::unique_ptr<Skybox> Renderer::s_skybox = nullptr;   

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // world grid stuff
    glGenVertexArrays(1, &s_LineVAO);
    glGenBuffers(1, &s_LineVBO);
    
    glBindVertexArray(s_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    s_lineShader = std::make_unique<Shader>("shaders/simple_color.vert", "shaders/simple_color.frag");

    // skybox initialization
    std::vector<std::string> faces
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };
    s_skybox = std::make_unique<Skybox>(faces);
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::submit(Shader& shader, Model& model, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();

    // set the view and projection matrices
    // in the shader
    shader.setMat4("u_View", view);
    shader.setMat4("u_Projection", projection);
    shader.setMat4("u_Model", model.getModelMatrix());

    model.draw(shader);

}

void Renderer::beginScene(glm::mat4& view, glm::mat4& projection) {
    // draw skybox first
    if(s_skybox) {
        s_skybox->draw(view, projection);
    }

    drawGrid(view, projection);
    drawAxes(view, projection);
}

void Renderer::drawGrid(const glm::mat4& view, const glm::mat4& projection) {
    int size = 50; // Total size 100x100
    for(int i = -size; i <= size; i++) {
        // Draw lines along X
        renderLine(glm::vec3(i, 0, -size), glm::vec3(i, 0, size), glm::vec3(0.3f), view, projection); // Pass view, projection
        // Draw lines along Z
        renderLine(glm::vec3(-size, 0, i), glm::vec3(size, 0, i), glm::vec3(0.3f), view, projection); // Pass view, projection
    }
}

void Renderer::drawAxes(const glm::mat4& view, const glm::mat4& projection) {
    static Shader axisShader("shaders/simple_color.vert", "shaders/simple_color.frag");

    axisShader.use();
    axisShader.setMat4("u_View", view);
    axisShader.setMat4("u_Projection", projection);

    glClear(GL_DEPTH_BUFFER_BIT); // Clear depth only so axes draw on top of everything
    glLineWidth(3.0f);

    // X - Red
    renderLine(glm::vec3(0,0,0), glm::vec3(5,0,0), glm::vec3(1,0,0), view, projection); // Pass view, projection
    // Y - Green
    renderLine(glm::vec3(0,0,0), glm::vec3(0,5,0), glm::vec3(0,1,0), view, projection); // Pass view, projection
    // Z - Blue
    renderLine(glm::vec3(0,0,0), glm::vec3(0,0,5), glm::vec3(0,0,1), view, projection); // Pass view, projection

    glLineWidth(1.0f); // Reset line width
}

void Renderer::renderLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, const glm::mat4& view, const glm::mat4& projection) {
    s_lineShader->use();
    s_lineShader->setMat4("u_View", view);
    s_lineShader->setMat4("u_Projection", projection);
    s_lineShader->setMat4("u_Model", glm::mat4(1.0f)); // Grid/Axes are in world space
    s_lineShader->setVec3("u_Color", color);

    float vertices[] = { start.x, start.y, start.z, end.x, end.y, end.z };

    glBindVertexArray(s_LineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_LineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Update existing buffer
    
    glDrawArrays(GL_LINES, 0, 2);
}

// ai slop right here
void Renderer::drawViewportGizmo(const glm::mat4& cameraRotation, const glm::mat4& projection) {
    // 1. Save current viewport to restore it later
    GLint oldViewport[4];
    glGetIntegerv(GL_VIEWPORT, oldViewport);

    // 2. Set viewport to a small square in the top-right or bottom-left corner
    int gizmoSize = 100;
    glViewport(10, 10, gizmoSize, gizmoSize); // (x, y, width, height)

    // 3. Clear the depth buffer so it draws ON TOP of the cottage
    glClear(GL_DEPTH_BUFFER_BIT);

    // 4. Create a View Matrix that ONLY has rotation (removes camera position)
    // This makes the gizmo rotate as you move the mouse, but it stays centered in its tiny box
    glm::mat4 gizmoView = glm::mat4(glm::mat3(cameraRotation)); 
    
    // 5. Use an Orthographic projection so the axes don't look "skewed"
    glm::mat4 gizmoProj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10.0f);

    // 6. Draw the Axes (Reuse your existing logic)
    // X - Red
    renderLine(glm::vec3(0,0,0), glm::vec3(1,1,0), glm::vec3(1,0,0), gizmoView, gizmoProj); 
    // Y - Green
    renderLine(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,1,0), gizmoView, gizmoProj); 
    // Z - Blue
    renderLine(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,0,1), gizmoView, gizmoProj); 

    // 7. Restore the original viewport for the rest of the engine
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}

void Renderer::shutdown() {
}