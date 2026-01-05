#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal; 
in vec3 FragPos; // Note: You'll need to pass this from Vertex Shader for specular

uniform vec4 u_BaseColor;
uniform bool u_HasTexture;

// Texture Samplers
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1; 

uniform vec3 viewPos; // Camera position needed for specular highlights

void main() {
    // 1. Fetch Diffuse Color
    vec4 color;
    if(u_HasTexture) {
        color = texture(texture_diffuse1, TexCoords);
    } else {
        color = u_BaseColor;
    }

    // 2. Ambient Lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * color.rgb;

    // 3. Diffuse Lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color.rgb;

    // 4. Specular Lighting (The new part!)
    float specularStrength = texture(texture_specular1, TexCoords).r; // Use the map
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 is shininess
    vec3 specular = specularStrength * spec * vec3(1.0); // White highlights

    // Final Color
    FragColor = vec4(ambient + diffuse + specular, color.a);
}