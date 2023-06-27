#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform Light light;
uniform vec3 viewPos;

void main(){
        FragColor = vec4(1.0,0.0,0.0, 1.0);
}