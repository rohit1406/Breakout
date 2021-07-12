//
//  ResourceManager.hpp
//  Breakout
//
//  Created by Apple on 01/07/21.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <stdio.h>

#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "Texture.hpp"
#include "Shader.hpp"

// A static singleton class that hosts several functions to load textures and shaders.
// Each loaded texture and shaders is also stored for future use by string handles.
// All functions and resources are static and no public constructor is defined.
class ResourceManager{
public:
    // Resource storage
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;
    
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code.
    // if gShaderFile is not null, it also loads geometry shader
    static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    
    // Retrieve a stored shader
    static Shader getShader(std::string name);
    
    // loads (and generates) a texture from file
    static Texture2D loadTexture(const char* file, bool alpha, std::string name);
    
    // Retrieve a stored texture
    static Texture2D getTexture(std::string name);
    
    // Properly de-allocate all loaded resources
    static void clear();
private:
    // private constructor, that is we do not want any actual resource manager objects.
    // It's members and functions should be publicly available (static)
    ResourceManager(){}
    
    // Load and Generate shaders from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    
    // Load a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};
#endif /* ResourceManager_hpp */
