#version 430

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = vec4(TexCoord , 0.0f, 1.0f);
}
