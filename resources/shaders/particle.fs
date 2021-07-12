#version 330 core       // Represents OpenGL version 3.3

// In Parameters
in vec2 TexCoords;
in vec4 ParticleColor;

// Uniforms
uniform sampler2D usSprite;

// Out Parameters
out vec4 color;

void main(){
    color = texture(usSprite, TexCoords) * ParticleColor;
}
