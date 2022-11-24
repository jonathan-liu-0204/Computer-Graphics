#version 430
layout (location = 0) in vec3 position;

uniform mat4 LightViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = LightViewMatrix * ModelMatrix * vec4(position, 1.0f);
}