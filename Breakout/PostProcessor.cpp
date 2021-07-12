//
//  PostProcessor.cpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#include "PostProcessor.hpp"

// FUNCTIONS
// -- Constructors/Destructors
PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
    : postProcessingShader(shader), texture(), width(width), height(height), confuse(false), chaos(false), shake(false){
        // Initialize Renderbuffer / Framebuffer object
        glGenFramebuffers(1, &this->MSFBO);
        glGenFramebuffers(1, &this->FBO);
        glGenRenderbuffers(1, &this->RBO);
        
        // Initialize renderbuffer storage with a multisampled color buffer ( don't need a depth/stencil buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        // Allocate storage for renderbuffer object
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
        // Attach MS render buffer object to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            LOGGER("ERROR::POSTPROCESSOR:: Failed to initialize MSFBO");
        }
        
        // Also initialize the FBO/texture to blit multisampled color-buffer to; Used for shader operations ( for postprocessing effects )
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        this->texture.generate(width, height, NULL);
        // attach texture to framebuffer as a color attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.ID, 0);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            LOGGER("ERROR::POSTPROCESSOR:: Failed to initialize FBO");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        LOGGER("POSTPROCESSOR:: Done with framebuffers initialisation.");
        
        // Initialise Render data and uniforms
        this->initRenderData();
        this->postProcessingShader.setInteger("usScene", 0, true);
        
        float offset = 1.0f / 300.0f;
        float offsets[9][2] = {
            {-offset, offset},      // Top-Left
            {0.0f, offset},         // Top-Center
            {offset, offset},       // Top-Right
            {-offset, 0.0f},        // center-left
            {0.0f, 0.0f},           // center-center
            {offset, 0.0f},         // center - right
            {-offset, -offset},     // bottom-left
            {0.0f, -offset},        // bottom-center
            {offset, -offset}       // bottom-right
        };
        glUniform2fv(glGetUniformLocation(this->postProcessingShader.ID, "uOffsets"), 9, (float *) offsets);
        
        int edgeKernel[9] = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };
        glUniform1iv(glGetUniformLocation(this->postProcessingShader.ID, "uEdgeKernel"), 9, edgeKernel);
        
        float blurKernel[9] = {
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
        };
        glUniform1fv(glGetUniformLocation(this->postProcessingShader.ID, "uBlurKernel"), 9, blurKernel);
}

PostProcessor::~PostProcessor(){
    
}

// -- Initialization functions
// Initialize quad for rendering postprocessing texture
void PostProcessor::initRenderData(){
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// -- Rendering Functions
// prepares the postprocessor's framebuffer operations before rendering the game
void PostProcessor::beginRender(){
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// should be called after rendering the game, so it stores all the rendered data into a texture object
void PostProcessor::endRender(){
    // Now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBlitFramebuffer(0, 0, this->width, this->height,
                      0, 0, this->width, this->height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // binds both READ and WRITE framebuffer to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// renders the PostProcessor texture quad (as a screen-encompassing large sprite)
void PostProcessor::render(float time){
    // Set Uniforms and Options
    this->postProcessingShader.use();
    this->postProcessingShader.setFloat("uTime", time);
    this->postProcessingShader.setInteger("uConfuse", this->confuse);
    this->postProcessingShader.setInteger("uChaos", this->chaos);
    this->postProcessingShader.setInteger("uShake", this->shake);
    
    // Render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->texture.bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
