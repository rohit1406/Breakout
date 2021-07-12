#version 330            // Corresponds to OpenGL version 3.3

// In Attributes
layout(location = 0) in vec4 aVertex;   // <vec2 Pos, vec2 Tex>

// Uniforms
uniform mat4 uModel;
uniform mat4 uProjection;

// Out Values
out vec2 TexCoords;

void main(){
    gl_Position = uProjection * uModel * vec4(aVertex.xy, 0.0f, 1.0f);
    TexCoords = aVertex.zw;
}
