#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos;
    // We convert the view matrix to a mat3 to remove any translation (camera position)
    // so the skybox always stays centered around the camera.
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    
    // Setting z to w ensures that after the perspective divide, z = 1.0.
    // This places the skybox at the absolute back of the depth buffer.
    gl_Position = pos.xyww;
}