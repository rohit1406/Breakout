#version 330            // Corresponds to OpenGL version 3.3

// In Values
in vec2 TexCoords;

// Uniforms
uniform sampler2D usSpriteImage;
uniform vec3 uSpriteColor;

// Out Values
out vec4 color;

void main(){
    color = vec4(uSpriteColor, 1.0f) * texture(usSpriteImage, TexCoords);
}
