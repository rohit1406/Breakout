//
//  SpriteRenderer.hpp
//  Breakout
//
//  Created by Apple on 03/07/21.
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

#include <stdio.h>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

class SpriteRenderer{
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    
    void drawSprite(Texture2D& texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    unsigned int quadVAO;
    
    void intialiseRenderData();
};
#endif /* SpriteRenderer_hpp */
