//
//  main.cpp
//  Breakout
//
//  Created by Apple on 28/06/21.
//

#include <iostream>
#include <GL/glew.h>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "Game.hpp"
#include "ResourceManager.hpp"

// GLFW Callback functions declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game breakout(SCREEN_WIDTH, SCREEN_HEIGHT);


int main(int argc, const char * argv[]) {
    LOGGER("Starting application");
    //Initialize GLFW
    if(!glfwInit()){
        LOGGER("Failed to initialise GLFW! Terminating GLFW.");
        glfwTerminate();
        return 1;
    }
    LOGGER("GLFW initialised.");
    
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);
    LOGGER("OpenGL properties set with OpenGL version 3.3");
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    if(!window){
        LOGGER("Failed to create GLFW Window! Terminating GLFW.");
        glfwTerminate();
        return 1;
    }
    LOGGER("Main Window Created.");
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // GLEW: load all OpenGL function pointers
    if(glewInit() != GLEW_OK){
        LOGGER("Failed to initialise GLEW!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    LOGGER("GLEW Initialised.");
    
    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    LOGGER("Viewport set to "+std::to_string(SCREEN_WIDTH)+"*"+std::to_string(SCREEN_HEIGHT));
    //glfwSetWindowUserPointer(window, this);
    LOGGER("Window Initialisation Completed.");
    
    // Initialize Game
    breakout.init();
    
    // Deltatime Variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while(!glfwWindowShouldClose(window)){
        // Calculat Delta time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        glfwPollEvents();
        
        // Manage User input
        breakout.processInput(deltaTime);
        
        // Update game state
        breakout.update(deltaTime);
        
        // Render
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        breakout.render();
        
        glfwSwapBuffers(window);
    }
    
    // Delete all loaded resources
    ResourceManager::clear();
    
    glfwTerminate();
    LOGGER("Good Bye!");
    LOGGER("------------------------------------------------------");
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // When the user presses escape key, we set the window should close property to true, closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    
    if(key > 0 && key < 1024){
        if(action == GLFW_PRESS){
            breakout.keys[key] = true;
        }else if(action == GLFW_RELEASE){
            breakout.keys[key] = false;
            breakout.keysProcessed[key] = false;
        }
    }
}
