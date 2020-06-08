//
//  main.cpp
//  Breakout
//
//  Created by Ben on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Game.hpp"

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);


int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glad_glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

    glfwSetKeyCallback(window, key_callback);

    // OpenGL configuration
    glad_glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//    glad_glEnable(GL_CULL_FACE);
    glad_glEnable(GL_BLEND);
    glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Game
    Breakout.Init();

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        Breakout.ProcessInput(deltaTime);
        
        Breakout.Update(deltaTime);

        // Render
        glad_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            Breakout.Keys[key] = GL_TRUE;
        }
        else if (action == GLFW_RELEASE) {
            Breakout.Keys[key] = GL_FALSE;
            Breakout.KeysProcessed[key] = GL_FALSE;
        }
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
