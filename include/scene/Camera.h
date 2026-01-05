#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
    public:
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        //euler angles
        float m_yaw;
        float m_pitch;

        // movement options
        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_zoom;
    
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
               float yaw = -90.0f, 
               float pitch = 0.0f);


        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);

        void processKeyboard(Camera_Movement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset);
        void processMouseScroll(float yoffset); // for zooming and such 
        void preProcessMouseMovement();

        ~Camera();
    
    private:
        void updateCameraVectors();
};

#endif // CAMERA_H