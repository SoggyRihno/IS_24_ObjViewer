#version 460 core
out vec4 FragColor;

struct Material {
    vec3 Ka; //ambiant
    vec3 Kd; //diffuse
    vec3 Ks; //specular
    float Ns; // shininess

};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoord;

uniform vec3 cameraPos;
uniform vec3 matColor;
uniform int mode;
uniform int hasDiffuse;

uniform sampler2D diffuseTexture;

uniform Material material;
uniform Light light;

void main()
{
    if (mode == 1) {
        FragColor = vec4(matColor, 1.0f);
        return;
    }

    // ambient
    vec3 ambient = light.ambient * material.Ka * 2;

    // diffuse
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse;

    if (hasDiffuse == 1) {
        diffuse *= diff * texture(diffuseTexture, TexCoord).rgb;
        ambient = light.ambient * texture(diffuseTexture, TexCoord).rgb;
    } else {
        diffuse *= diff * material.Kd;
    }

    vec3 lighting = ambient + diffuse;
    FragColor = vec4(2 * lighting, 1);

}