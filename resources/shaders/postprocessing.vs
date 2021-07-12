#version 330 core       // Represents OpenGL version 3.3

// In Attributes
layout(location=0) in vec4 aVertex;   // <vec2 pos, vec2 texcoords>

// Uniforms
uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;
uniform float uTime;

// Out Parameters
out vec2 TexCoords;

void main(){
    gl_Position = vec4(aVertex.xy, 0.0f, 1.0f);
    vec2 texture = aVertex.zw;
    
    if(uChaos){
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(uTime) * strength, texture.y + cos(uTime) * strength);
        TexCoords = pos;
    }else if(uConfuse){
        TexCoords = vec2(1.0f - texture.x, 1.0f - texture.y);
    }else{
        TexCoords = texture;
    }
    
    if(uShake){
        float strength = 0.01f;
        gl_Position.x += cos(uTime * 10) * strength;
        gl_Position.y += cos(uTime * 15) * strength;
    }
}
