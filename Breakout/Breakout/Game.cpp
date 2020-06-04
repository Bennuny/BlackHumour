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
    _currentLevel(3),
    _pQuadRenderer(nullptr),
    _pPaddle(nullptr)
{
    _sprites.clear();
}

Game::~Game()
{
    if (_pQuadRenderer) {
        delete _pQuadRenderer;
    }
    
    for (GameLevel *pLevel : _vGameLevels) {
        delete pLevel;
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

    for (int i = 0; i < 4; i++) {
        GameLevel *pLevel = new GameLevel();

        char repath[128] = "";
        sprintf(repath, "Level/level_%d", i+1);
        pLevel->Load(repath, _pQuadRenderer, _windowWidth, _windowHeight/2);
        _vGameLevels.push_back(pLevel);
    }
    
    CreateSprite("Texture/background.jpg");
    _pPaddle = CreateSprite("Texture/paddle.png");
    _pPaddle->SetSize(100.0f, 20.0f);
    _pPaddle->SetPosition(_windowWidth/2 - _pPaddle->GetWidth()/2, _windowHeight-_pPaddle->GetHeight());
}

Sprite2D* Game::CreateSprite(std::string file)
{
    Sprite2D *sprite = new Sprite2D(file, _pQuadRenderer);
    _sprites.push_back(sprite);

    return sprite;
}

void Game::ProcessInput(GLfloat dt)
{
    
}

void Game::Update(GLfloat dt)
{
    
}

void Game::Render()
{
    for (Sprite2D* sprite : _sprites) {
        sprite->Draw();
    }
    _vGameLevels[_currentLevel]->Draw();
}
