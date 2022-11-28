#version 430

layout(location = 0) in vec3 position;

out vec3 TexCoord;

uniform mat4 Projection;
uniform mat4 ViewMatrix;

// TODO#1-2: vertex shader / fragment shader
// 1. properly set gl_Position and TexCoord in vertex shader
// 2. properly set color with skybox texture and input from vertex shader

void main()
{
	TexCoord = position;
	mat4 view = mat4(mat3(ViewMatrix));
	gl_Position = Projection * view * vec4(position, 1.0);
}