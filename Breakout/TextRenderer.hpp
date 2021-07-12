//
//  TextRenderer.hpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#ifndef TextRenderer_hpp
#define TextRenderer_hpp

#include <stdio.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"
#include "ResourceManager.hpp"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2   size;      // size of glyph
    glm::ivec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer{
public:
    // holds a list of pre-compiled Characters
    std::map<char, Character> characters;
    // shader used for text rendering
    Shader textShader;
    // constructor
    TextRenderer(unsigned int width, unsigned int height);
    // pre-compiles a list of characters from the given font
    void load(std::string font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    // render state
    unsigned int VAO, VBO;
};
#endif /* TextRenderer_hpp */
