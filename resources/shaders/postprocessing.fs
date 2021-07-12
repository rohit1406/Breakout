#version 330 core       // Represents OpenGL version 3.3

// In Parameters
in vec2 TexCoords;

// Uniforms
uniform sampler2D usScene;
uniform vec2 uOffsets[9];
uniform int uEdgeKernel[9];
uniform float uBlurKernel[9];

uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;

// Out Parameters
out vec4 color;

void main(){
    color = vec4(0.0f);
    
    vec3 sample[9];
    // Sample from texture offsets if using convolution matrix
    if(uChaos || uShake){
        for(int i=0; i<9; i++){
            sample[i] = vec3(texture(usScene, TexCoords.st + uOffsets[i]));
        }
    }
    
    // Process Effects
    if(uChaos){
        for(int i=0; i<9; i++){
            color += vec4(sample[i] * uEdgeKernel[i], 0.0f);
        }
        color.a = 1.0f;
    }else if(uConfuse){
        color = vec4(1.0f - texture(usScene, TexCoords).rgb, 1.0f);
    }else if(uShake){
        for(int i=0; i<9; i++){
            color += vec4(sample[i] * uBlurKernel[i], 0.0f);
        }
        color.a = 1.0f;
    }else{
        color = texture(usScene, TexCoords);
    }
}
