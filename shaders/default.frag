#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal; // You're already passing this in!

uniform vec4 u_BaseColor;
uniform bool u_HasTexture;
uniform sampler2D texture_diffuse1;

void main() {
    vec4 color;
    if(u_HasTexture) {
        color = texture(texture_diffuse1, TexCoords);
    } else {
        color = u_BaseColor;
    }

    // Simple Directional Lighting (coming from above/front)
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.2); // 0.2 is ambient light so it's not pitch black

    FragColor = vec4(color.rgb * diff, color.a);
}