//
//  PostProcessor.hpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#ifndef PostProcessor_hpp
#define PostProcessor_hpp

#include <stdio.h>

#include "Shader.hpp"
#include "Texture.hpp"

class PostProcessor{
public:
    // STATES
    // Options
    bool confuse, chaos, shake;
    
    // FUNCTIONS
    // -- Constructors/Destructors
    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    ~PostProcessor();
    
    // -- Rendering Functions
    // prepares the postprocessor's framebuffer operations before rendering the game
    void beginRender();
    // should be called after rendering the game, so it stores all the rendered data into a texture object
    void endRender();
    // renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void render(float time);
private:
    // STATES
    Shader postProcessingShader;
    Texture2D texture;
    unsigned int width, height;
    
    // -- Render States
    // MSFBO - Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int MSFBO, FBO;
    unsigned int RBO;       // used for multi-sampled color buffer
    unsigned int VAO;
    
    // FUNCTIONS
    // -- Initialization functions
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};
#endif /* PostProcessor_hpp */
