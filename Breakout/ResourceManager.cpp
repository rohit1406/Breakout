//
//  ResourceManager.cpp
//  Breakout
//
//  Created by Apple on 01/07/21.
//

#include "ResourceManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Initialise static variables
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code.
// if gShaderFile is not null, it also loads geometry shader
Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name){
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

// Retrieve a stored shader
Shader ResourceManager::getShader(std::string name){
    return shaders[name];
}

// loads (and generates) a texture from file
Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name){
    textures[name] = loadTextureFromFile(file, alpha);
    return  textures[name];
}

// Retrieve a stored texture
Texture2D ResourceManager::getTexture(std::string name){
    return  textures[name];
}

// Properly de-allocate all loaded resources
void ResourceManager::clear(){
    // Properly delete all shaders
    for(auto iter: shaders){
        glDeleteProgram(iter.second.ID);
    }
    
    // Properly delete all textures
    for(auto iter: textures){
        glDeleteTextures(1, &iter.second.ID);
    }
}

// Load and Generate shaders from file
Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile){
    LOGGER("Loading Shaders: "+std::string(vShaderFile)+" "+std::string(fShaderFile)+" "+std::string(gShaderFile== nullptr ? "" :  gShaderFile));
    // Retrieve the vertex/fragment source code from file path
    std::string vertexCode, fragmentCode, geometryCode;
    try{
        // Open the files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        
        // Read file's buffer contents into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        
        // Close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        
        // Convert Stream into String
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if(gShaderFile != nullptr){
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }catch(std::exception e){
        LOGGER("ERROR:SHADER:: Failed to load shader files");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // Now Create the Shader object from the shader code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile!=nullptr ? gShaderCode : nullptr);
    return shader;
}

// Load a single texture from file
Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha){
    LOGGER("Loading Texture "+std::string(file)+" with alpha = "+std::to_string(alpha));
    // Create Texture2D Object
    Texture2D texture;
    if(alpha){
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }
    // Load Image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // Now generate the textures
    texture.generate(width, height, data);
    // Finally free the image data
    stbi_image_free(data);
    return texture;
}
