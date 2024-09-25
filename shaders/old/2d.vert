#version 460 core
layout (location = 0) in vec3 postion;

void main()
{
    gl_Position = vec4(postion, 1.0);
}