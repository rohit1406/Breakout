#version 330                // OpenGL version 3.3

layout(location = 0) in vec4 vPosTex;

out vec2 outTexCoords;

uniform mat4 uProjection;

void main(){
    gl_Position = uProjection * vec4(vPosTex.xy, 0.0, 1.0);
    outTexCoords = vPosTex.zw;
}
