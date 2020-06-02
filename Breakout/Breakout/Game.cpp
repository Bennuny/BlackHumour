//
//  Game.cpp
//  Breakout
//
//  Created by Ben on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Game.hpp"

#include "Shader.hpp"
#include "ResourceManager.hpp"

Game::Game(GLuint width, GLuint height):
    _windowWidth(width),
    _windowHeight(height)
{
    
}

Game::~Game()
{
    
}

void Game::Init()
{
    glm::mat4 ortho = glm::ortho(0.0f, static_cast<GLfloat>(_windowWidth), 0.0f, static_cast<GLfloat>(_windowHeight), -1.0f, 1.0f);
    
    Shader shader = ResourceManager::GetShader(ResourceManager::SHADER_MODEL_TEX_COLOR);
//    shader.Use();
//    shader.SetMatrix4("projection", ortho);
//    shader.SetInteger("image", 0);

}

void Game::ProcessInput(GLfloat dt)
{
    
}

void Game::Update(GLfloat dt)
{
    
}

void Game::Render()
{
    
}
