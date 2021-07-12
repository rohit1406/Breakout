#version 330 core       // Represents OpenGL version 3.3

// In Attributes
layout(location=0) in vec4 aVertex;     // <vec2 position, vec2 texcoords>

// Uniforms
uniform mat4 uProjection;
uniform vec2 uOffset;
uniform vec4 uColor;

// Out Parameters
out vec2 TexCoords;
out vec4 ParticleColor;

void main(){
    float scale = 10.0f;
    TexCoords = aVertex.zw;
    ParticleColor = uColor;
    gl_Position = uProjection * vec4((aVertex.xy * scale)+ uOffset, 0.0f, 1.0f);
}
