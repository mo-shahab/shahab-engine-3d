#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal; 
in vec3 FragPos; // Note: You'll need to pass this from Vertex Shader for specular

uniform vec4 u_BaseColor;

uniform bool u_HasTexture;
uniform bool u_HasDiffuse;
uniform bool u_HasSpecular;
uniform bool u_HasNormalMap;  

// Texture Samplers
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1; 

uniform vec3 viewPos; // Camera position needed for specular highlights

void main() {
   
    vec4 color;
    vec3 norm = normalize(Normal);
    float specularStrength = 0.5; // default specular strength

    if(u_HasTexture) {
        color = texture(texture_diffuse1, TexCoords);
    } else {
        color = u_BaseColor;
    }

    // specular related 
    if(u_HasSpecular) {
        specularStrength = texture(texture_specular1, TexCoords).r; // Use the map
    }
    else  {
        specularStrength = 0.5; // default value
    }

    // normal mapping related
    if(u_HasNormalMap) {
        // You'll need TBN logic here later, for now just normalize the input
        norm = normalize(Normal); 
    } else {
        norm = normalize(Normal);
    }

// ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * color.rgb;

// diffuse lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color.rgb;

    // specular lighting 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 is shininess
    vec3 specular = specularStrength * spec * vec3(1.0); // White highlights

    // Final Color
    FragColor = vec4(ambient + diffuse + specular, color.a);
}