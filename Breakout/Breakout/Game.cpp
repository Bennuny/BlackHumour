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
    _windowHeight(height),
    _pQuadRenderer(nullptr)
{
    _sprites.clear();
}

Game::~Game()
{
    if (_pQuadRenderer) {
        delete _pQuadRenderer;
    }
}

void Game::Init()
{
    glm::mat4 ortho = glm::ortho(0.0f, static_cast<GLfloat>(_windowWidth), static_cast<GLfloat>(_windowHeight), 0.0f, -1.0f, 1.0f);
//    glm::mat4 ortho = glm::ortho(0.0f, static_cast<GLfloat>(_windowWidth), 0.0f, static_cast<GLfloat>(_windowHeight), -1.0f, 1.0f);
    
    Shader shader = ResourceManager::GetShader(ResourceManager::SHADER_MODEL_TEX_COLOR);
    shader.Use();
    shader.SetMatrix4("projection", ortho);
    shader.SetInteger("image", 0);

    _pQuadRenderer = new Renderer(shader);
    
    Sprite2D sp("Texture/awesomeface.png", _pQuadRenderer);
    
    _sprites.push_back(sp);
}

void Game::ProcessInput(GLfloat dt)
{
    
}

void Game::Update(GLfloat dt)
{
    
}

void Game::Render()
{
    for (Sprite2D sprite : _sprites) {
        sprite.Draw();
    }
}
