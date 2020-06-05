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

void Game::ResetPlayer()
{
    glm::vec2 paddlePos(_windowWidth/2 - _pPaddle->GetWidth()/2, _windowHeight-_pPaddle->GetHeight());
    _pPaddle->SetPosition(paddlePos);
}

void Game::ResetLevel()
{
    _vGameLevels[_currentLevel]->Reset();
}

void Game::GameOver()
{
    ResetPlayer();
    ResetLevel();
    ResetBall();
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

        if (!brick.IsDestroyed()) {
            Collision collision = CheckCollisionAABBCycle(*_pBall, brick);
            if (!std::get<0>(collision)) {
                continue;
            }

            if (!brick.IsSolid()) {
                brick.SetDestroyed(GL_TRUE);
            }
            
            Direction dir = std::get<1>(collision);
            glm::vec2 diff_vector = std::get<2>(collision);
            glm::vec2 ballPos = _pBall->GetPosition();
            
            if (dir == LEFT || dir == RIGHT) {
                _pBall->ReverseVelocityX();

                GLfloat penetration = _pBall->GetRadius() - std::abs(diff_vector.x);
                if (dir == LEFT) {
                    _pBall->SetPosition(ballPos.x + penetration, ballPos.y);
                }
                else {
                    _pBall->SetPosition(ballPos.x - penetration, ballPos.y);
                }
            }
            else {
                _pBall->ReverseVelocityY();

                GLfloat penetration = _pBall->GetRadius() - std::abs(diff_vector.y);
                if (dir == UP) {
                    _pBall->SetPosition(ballPos.x, ballPos.y - penetration);
                }
                else {
                    _pBall->SetPosition(ballPos.x, ballPos.y + penetration);
                }
            }
        }
    }

    if (!_pBall->IsStuck()) {
        Collision withPaddle = CheckCollisionAABBCycle(*_pBall, *_pPaddle);
        if (std::get<0>(withPaddle)) {
            GLfloat centerBoard = _pPaddle->GetPosition().x + _pPaddle->GetWidth() / 2;
            GLfloat distance = (_pBall->GetPosition().x + _pBall->GetRadius()) - centerBoard;
            GLfloat percentage = distance / (_pPaddle->GetWidth() / 2);
            
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = -_pBall->GetVelocity();
            
            glm::vec2 vel(0.0f);
            vel.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            vel.y = -1 * abs(oldVelocity.y);
            vel = glm::normalize(vel) * glm::length(oldVelocity);

            _pBall->SetVelocity(vel);
        }
    }
}

GLboolean Game::CheckCollisionAABB(GameObject &one, GameObject &two)
{
    bool collisionX = one.GetPosition().x + one.GetWidth() >= two.GetPosition().x && two.GetPosition().x + two.GetWidth() >= one.GetPosition().x;
    
    bool collisionY = one.GetPosition().y + one.GetHeight() >= two.GetPosition().y && two.GetPosition().y + two.GetHeight() >= one.GetPosition().y;
    
    return collisionX && collisionY;
}

Collision Game::CheckCollisionAABBCycle(BallObject &ball, Node &brick)
{
    glm::vec2 center(ball.GetPosition().x + ball.GetRadius(), ball.GetPosition().y + ball.GetRadius());
    
    glm::vec2 aabb_half_extents(brick.GetWidth()/2, brick.GetHeight()/2);
    glm::vec2 aabb_center(brick.GetPosition().x + aabb_half_extents.x, brick.GetPosition().y + aabb_half_extents.y);
    
    glm::vec2 difference = center - aabb_center;

    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    
    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;
    
    GLfloat length = glm::length(difference);
    
    if (length <= ball.GetRadius()) {
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    }
    else {
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f, 0.0f));
    }
}

Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };
    
    GLfloat max = 0.0f;
    GLuint  best_match = -1;
    
    glm::vec2 normal_target = glm::normalize(target);
    for (GLuint i = 0; i < DIR_COUNT; i++) {
        GLfloat dot_product = glm::dot(normal_target, compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    
    return Direction(best_match);
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
    
    if (_pBall->GetPosition().y >= _windowHeight) {
        GameOver();
        return;
    }
    
    DoCollision();
}

void Game::Render()
{
    if (_state == GAME_ACTIVE) {
        for (Sprite2D* sprite : _sprites) {
            sprite->Draw();
        }
    }
}
