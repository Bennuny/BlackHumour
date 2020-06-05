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

const GLfloat   PADDLE_VELOCITY = 500.0f;   // 500px/s
const GLfloat   BALL_RADIUS = 12.5f;

const glm::vec2 INITIAL_BALL_VELOCITY(-100.0f, -350.0f);

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
    
    _pBg = CreateSprite("Texture/background.jpg");
    _pPaddle = CreateSprite("Texture/paddle.png");
    _pPaddle->SetSize(100.0f, 20.0f);
    glm::vec2 paddlePos(_windowWidth/2 - _pPaddle->GetWidth()/2, _windowHeight-_pPaddle->GetHeight());
    _pPaddle->SetPosition(paddlePos);
    _pBall = CreateBallObject("Texture/awesomeface.png");
    ResetBall();
    
    SetLevel(0);
}

void Game::ResetBall()
{
    glm::vec2 paddlePos = _pPaddle->GetPosition();
    _pBall->SetRadius(BALL_RADIUS);
    _pBall->SetStuck(GL_TRUE);
    _pBall->Reset(glm::vec2(paddlePos.x + _pPaddle->GetWidth()/2 - _pBall->GetWidth()/2 , paddlePos.y - _pBall->GetHeight()), INITIAL_BALL_VELOCITY);
    _pBall->SetMaxX(_windowWidth - _pBall->GetWidth());
}

void Game::SetLevel(unsigned int levelIdx)
{
    _currentLevel = levelIdx;
    
    GameLevel* currentLevel = _vGameLevels[_currentLevel];
    
    _pBg->ClearAllChildren();
    _pBg->AddChild(currentLevel);
}

Sprite2D* Game::CreateSprite(std::string file)
{
    Sprite2D *sprite = new Sprite2D(file, _pQuadRenderer);
    _sprites.push_back(sprite);

    return sprite;
}

BallObject* Game::CreateBallObject(std::string file) {
    BallObject *ball = new BallObject(file, _pQuadRenderer);
    _sprites.push_back(ball);

    return ball;
}

void Game::DoCollision()
{
    std::vector<GameObject>& bricks = _vGameLevels[_currentLevel]->Bricks();
    for (GameObject &brick : bricks) {
        
//        if (!brick.IsDestroyed() && CheckCollisionAABB(brick, *_pBall)) {
        if (!brick.IsDestroyed() && CheckCollisionAABBCycle(*_pBall, brick)) {
            if (!brick.IsSolid()) {
                brick.SetDestroyed(GL_TRUE);
            }
        }
    }
}

GLboolean Game::CheckCollisionAABB(GameObject &one, GameObject &two)
{
    bool collisionX = one.GetPosition().x + one.GetWidth() >= two.GetPosition().x && two.GetPosition().x + two.GetWidth() >= one.GetPosition().x;
    
    bool collisionY = one.GetPosition().y + one.GetHeight() >= two.GetPosition().y && two.GetPosition().y + two.GetHeight() >= one.GetPosition().y;
    
    return collisionX && collisionY;
}

GLboolean Game::CheckCollisionAABBCycle(BallObject &ball, GameObject &brick)
{
    glm::vec2 center(ball.GetPosition().x + ball.GetRadius(), ball.GetPosition().y + ball.GetRadius());
    
    glm::vec2 aabb_half_extents(brick.GetWidth()/2, brick.GetHeight()/2);
    glm::vec2 aabb_center(brick.GetPosition().x + aabb_half_extents.x, brick.GetPosition().y + aabb_half_extents.y);
    
    glm::vec2 difference = center - aabb_center;

    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    
    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;
    
    GLfloat length = glm::length(difference);
    
    return length <= ball.GetRadius();
}

void Game::ProcessInput(GLfloat dt)
{
    if (_state == GAME_ACTIVE) {
        if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_D]) {
            glm::vec2 pos = _pPaddle->GetPosition();
                
            float movex = PADDLE_VELOCITY * dt;
            if (Keys[GLFW_KEY_A]) {
                pos.x -= movex;
                pos.x = pos.x < 0 ? 0 : pos.x;
            }
            else {
                pos.x += movex;
                int max_x = _windowWidth - _pPaddle->GetWidth();
                pos.x = pos.x > max_x ? max_x : pos.x;
            }
            _pPaddle->SetPosition(pos.x, pos.y);
            
            if (_pBall->IsStuck()) {
                ResetBall();
            }
        }
        else if (Keys[GLFW_KEY_SPACE]) {
            _pBall->SetStuck(GL_FALSE);
        }
    }
}

void Game::Update(GLfloat dt)
{
    _pBall->Move(dt);
    
    DoCollision();
}

void Game::Render()
{
    if (_state == GAME_ACTIVE) {
//        _vGameLevels[_currentLevel]->Draw();
        for (Sprite2D* sprite : _sprites) {
            sprite->Draw();
        }
    }
}
