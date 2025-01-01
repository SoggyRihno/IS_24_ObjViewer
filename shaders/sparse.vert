#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;

uniform mat4 model, view, projection;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 TexCoord;

void main()
{
    // Transform vertex to clip space
    gl_Position = projection * view * model * vec4(position, 1.0);

    // Transform position to world space for lighting calculations
    FragPos = vec3(model * vec4(position, 1.0));

    // Transform normal to world space using the normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    FragNormal = normalMatrix * normal;

    // Pass through texture coordinates
    TexCoord = tex;
}