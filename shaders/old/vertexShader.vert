#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 TexCoord;


uniform mat4 model, view, projection;


void main() {
    gl_Position = projection * view * model * vec4(Position, 1.0);

    FragPos = vec3(model * vec4(Position, 1.0));
    FragNormal = mat3(transpose(inverse(model))) * Normal;
    TexCoord = aTexCoord;
}


