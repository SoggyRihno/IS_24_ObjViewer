#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoord;

struct Light {
    vec3 position;
    float _padding1;  // Required for std140 layout
    vec3 color;
    float intensity;
};

// Lights UBO
layout (std140, binding = 0) uniform LightsBlock {
    int numLights;
    Light lights[15];
};

// Material properties
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

uniform float d;

// Texture samplers
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D specularHighlightTexture;

// Texture flags
uniform uint texFlags;

// Constants for texture flags
const uint USE_AMBIENT_TEXTURE = 0u;
const uint USE_DIFFUSE_TEXTURE = 1u;
const uint USE_SPECULAR_TEXTURE = 2u;
const uint USE_SPECULAR_HIGHLIGHT_TEXTURE = 4u;

uniform vec3 viewPos;

void main() {
    vec3 normal = normalize(FragNormal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Get material properties from textures if enabled
    vec3 ambientMaterial = Ka;
    if ((texFlags & (1u << USE_AMBIENT_TEXTURE)) != 0u) {
        ambientMaterial *= texture(ambientTexture, TexCoord).rgb;
    }

    vec3 diffuseMaterial = Kd;
    if ((texFlags & (1u << USE_DIFFUSE_TEXTURE)) != 0u) {
        diffuseMaterial *= texture(diffuseTexture, TexCoord).rgb;
    }

    vec3 specularMaterial = Ks;
    if ((texFlags & (1u << USE_SPECULAR_TEXTURE)) != 0u) {
        specularMaterial *= texture(specularTexture, TexCoord).rgb;
    }

    float shininess = Ns;
    if ((texFlags & (1u << USE_SPECULAR_HIGHLIGHT_TEXTURE)) != 0u) {
        shininess *= texture(specularHighlightTexture, TexCoord).r;
    }

    // Lighting calculations
    for (int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float distance = length(lights[i].position - FragPos);

        float constant = 5;
        float linear = 0.022;
        float quadratic = 0.0019;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        // Ambient term
        ambient += lights[i].color * lights[i].intensity * 0.1 * ambientMaterial;

        // Diffuse term
        float diff = max(dot(normal, lightDir), 0.0);
        diffuse += lights[i].color * lights[i].intensity * diff * diffuseMaterial * attenuation;

        // Specular term
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular += lights[i].color * lights[i].intensity * spec * specularMaterial * attenuation;
    }

    // Combine results
    vec3 result = ambient + diffuse + specular;


    result = clamp(result, .05, 1);

    FragColor = vec4(result, d);
}