//
//  Texture.cpp
//  Breakout
//
//  Created by Apple on 01/07/21.
//

#include "Texture.hpp"

// Functions
// -- Constructor/Destructors
Texture2D::Texture2D():
            width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT),
            filterMin(GL_LINEAR), filterMag(GL_LINEAR){
    glGenTextures(1, &this->ID);
}
Texture2D::~Texture2D(){
    
}

// -- Others
void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data){
    this->width = width;
    this->height = height;
    
    // Activate the texture image
    glBindTexture(GL_TEXTURE_2D, this->ID);
    
    // Pass on the image data to texture image
    glTexImage2D(GL_TEXTURE_2D,
                 0,                     // Mipmap level
                 this->internalFormat,
                 this->width, this->height,
                 0,                     // Border - Kept for legacy reasons
                 this->imageFormat,
                 GL_UNSIGNED_BYTE,      // type of the texture data
                 data);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
    
    // Unbind texture now
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::bind() const{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
