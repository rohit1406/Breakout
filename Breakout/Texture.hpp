//
//  Texture.hpp
//  Breakout
//
//  Created by Apple on 01/07/21.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glew.h>

class Texture2D{
public:
    // States
    // Holds ID of the texture object
    unsigned int ID;
    // texture image dimentions
    unsigned int width, height;
    // Texture formats
    unsigned int internalFormat;
    unsigned int imageFormat;
    // Texture Configurations
    unsigned int wrapS;
    unsigned int wrapT;
    unsigned int filterMin;
    unsigned int filterMag;
    
    // Functions
    // -- Constructor/Destructors
    Texture2D();
    ~Texture2D();
    
    // -- Others
    void generate(unsigned int width, unsigned int height, unsigned char* data);
    void bind() const;
};
#endif /* Texture_hpp */
