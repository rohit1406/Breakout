//
//  SpriteRenderer.cpp
//  Breakout
//
//  Created by Apple on 03/07/21.
//

#include "SpriteRenderer.hpp"

void SpriteRenderer::intialiseRenderData(){
    // Vertex Data
    float vertices[] = {
        // POS          // TEX
        0.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 0.0f,     1.0f, 0.0f,
        0.0f, 0.0f,     0.0f, 0.0f,
        
        0.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, 0.0f,     1.0f, 0.0f
    };
    // Configure VAO and VBO
    unsigned int VBO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::SpriteRenderer(Shader &shader){
    this->shader = shader;
    this->intialiseRenderData();
}

SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position,
                glm::vec2 size, float rotate,
                                glm::vec3 color){
    // Use Shader
    this->shader.use();
    
    // Prepare Transformation Data
    glm::mat4 model = glm::mat4(1.0f);      // Identity Matrix
    model = glm::translate(model, glm::vec3(position, 0.0f));
    
    /**
     Because we specified the quad's vertices with (0,0) as the top-left coordinate, all rotations will rotate around this point of (0,0). The origin of rotation is at the top-left of the quad, which produces undesirable results. What we want to do is move the origin of rotation to the center of the quad so the quad neatly rotates around this origin, instead of rotating around the top-left of the quad. We solve this by translating the quad by half its size first, so its center is at coordinate (0,0) before rotating.
     
     Since we first scale the quad, we have to take the size of the sprite into account when translating to the sprite's center, which is why we multiply with the sprite's size vector. Once the rotation transformation is applied, we reverse the previous translation.

     Combining all these transformations, we can position, scale, and rotate each sprite in any way we like.
     */
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    // Render
    this->shader.setMatrix4("uModel", model);
    this->shader.setVector3f("uSpriteColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // Clean up
}
