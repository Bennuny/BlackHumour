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

#include <irrKlang/irrKlang.h>
using namespace irrklang;

#include <sstream>

#define SAFE_DELETE(p) if(p) { delete p; }

ISoundEngine *SoundEngine = createIrrKlangDevice();

const GLfloat   PADDLE_VELOCITY = 500.0f;   // 500px/s
const GLfloat   BALL_RADIUS = 12.5f;

const glm::vec2 INITIAL_BALL_VELOCITY(-100.0f, -350.0f);

Game::Game(GLuint width, GLuint height):
    _windowWidth(width),
    _windowHeight(height),
    _currentLevel(3),
    _pQuadRenderer(nullptr),
    _pParticleRenderer(nullptr),
    _pParticleManager(nullptr),
    _pPostProcessor(nullptr),
    _pPaddle(nullptr)
{
    _sprites.clear();
    
    _ShakeTime = 0.0f;
    
    _state = GAME_MENU;
    _Lives = 3;
}

Game::~Game()
{
    if (_pQuadRenderer) {
        delete _pQuadRenderer;
    }
    if (_pParticleRenderer) {
        delete _pParticleRenderer;
    }
    
    SAFE_DELETE(_pParticleManager);
    
    SAFE_DELETE(_pPostProcessor);
    
    SAFE_DELETE(_pTextRender);
    
    for (GameLevel *pLevel : _vGameLevels) {
        delete pLevel;
    }
}

void Game::Init()
{
    SoundEngine->play2D(ResourceManager::GetFullPath("Sound/breakout.ogg").c_str(), true);
    
    glm::mat4 ortho = glm::ortho(0.0f, static_cast<GLfloat>(_windowWidth), static_cast<GLfloat>(_windowHeight), 0.0f, -1.0f, 1.0f);
//    glm::mat4 ortho = glm::ortho(0.0f, static_cast<GLfloat>(_windowWidth), 0.0f, static_cast<GLfloat>(_windowHeight), -1.0f, 1.0f);

    Shader shader = ResourceManager::GetShader(ResourceManager::SHADER_MODEL_TEX_COLOR);
    shader.Use();
    shader.SetMatrix4("projection", ortho);
    shader.SetInteger("image", 0);
    
    Shader particleShader = ResourceManager::GetShader(ResourceManager::SHADER_PARTICLE);
    particleShader.Use();
    particleShader.SetMatrix4("projection", ortho);
    particleShader.SetInteger("image", 0);
    
    Shader textShader = ResourceManager::GetShader(ResourceManager::SHADER_TEXT_RENDER);
    textShader.Use();
    textShader.SetMatrix4("projection", ortho);
    textShader.SetInteger("image", 0);
    
    Shader shaderPostprocess = ResourceManager::GetShader(ResourceManager::SHADER_POST_PROCESSING);
    
    _pPostProcessor = new PostProcessor(shaderPostprocess, _windowWidth, _windowHeight);
    
    _pTextRender = new TextRenderer(textShader, ortho);
    _pTextRender->Load(ResourceManager::GetFullPath("Font/Arial Black.ttf"), 24);

    _pQuadRenderer = new Renderer(shader);
    _pParticleRenderer = new Renderer(particleShader);
    
    _pParticleManager = new ParticleManager(_pParticleRenderer, "Texture/particle.png", 500);

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
    
    _pBall->AddChild(_pParticleManager);
    
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
    
    _Lives = 3;
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
                SpawnPowerUps(brick);
                
                SoundEngine->play2D(ResourceManager::GetFullPath("Sound/bleep.wav").c_str());
            }
            else {
                SoundEngine->play2D(ResourceManager::GetFullPath("Sound/solid.wav").c_str());
                
                _pPostProcessor->Shake = GL_TRUE;
                _ShakeTime = 0.5f;
            }
            
            Direction dir = std::get<1>(collision);
            glm::vec2 diff_vector = std::get<2>(collision);
            glm::vec2 ballPos = _pBall->GetPosition();
            
            if (!(_pBall->PassThrough() && !brick.IsSolid())) {
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
    }

    if (!_pBall->IsStuck()) {
        Collision withPaddle = CheckCollisionAABBCycle(*_pBall, *_pPaddle);
        if (std::get<0>(withPaddle)) {
            SoundEngine->play2D(ResourceManager::GetFullPath("Sound/bleep.wav").c_str());
            
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
            
            if (_pBall->isSticky()) {
                _pBall->SetStuck(GL_TRUE);
                _pBall->SetSticky(GL_FALSE);
            }
        }
    }
    
    for (PowerUp &powup : _powerUPs) {
        if (!powup.IsDestroyed()) {
            if (powup.GetPosition().y >= _windowHeight) {
                powup.SetDestroyed(GL_TRUE);
            }
            if (CheckCollisionAABB(*_pPaddle, powup)) {
                ActivatePowUp(powup);

                powup.SetDestroyed(GL_TRUE);
                powup.SetActiviated(GL_TRUE);
                
                SoundEngine->play2D(ResourceManager::GetFullPath("Sound/powerup.wav").c_str());
            }
        }
    }
}

GLboolean Game::CheckCollisionAABB(Node &one, Node &two)
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
    else if (_state == GAME_MENU) {
        if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
            _state = GAME_ACTIVE;
            KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
        }
        
        if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W]) {
            _currentLevel = (_currentLevel + 1) % 4;
            KeysProcessed[GLFW_KEY_W] = GL_TRUE;
            SetLevel(_currentLevel);
        }
        
        if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S]) {
            KeysProcessed[GLFW_KEY_S] = GL_TRUE;
            if (_currentLevel > 0) {
                --_currentLevel;
            }
            else {
                _currentLevel = 3;
            }
            SetLevel(_currentLevel);
        }
    }
    else if (_state == GAME_WIN) {
        if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
            KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
            _pPostProcessor->Chaos = GL_FALSE;
            _state = GAME_MENU;
        }
    }
}

void Game::Update(GLfloat dt)
{
    _pBall->Move(dt);
    
    _pParticleManager->Update(dt, _pBall, 2, glm::vec2(_pBall->GetRadius() / 2));
    
    if (_pBall->GetPosition().y >= _windowHeight) {
//        GameOver();
//        return;
        
        --_Lives;
        if (_Lives == 0) {
            GameOver();
            _state = GAME_MENU;
        }
        ResetPlayer();
        ResetBall();
    }
    
    DoCollision();

    if (_pPostProcessor->Shake) {
        _ShakeTime -= dt;
        if (_ShakeTime <= 0.0f) {
            _pPostProcessor->Shake = GL_FALSE;
        }
    }
    
    UpdatePowerUps(dt);
    
    if (_state == GAME_ACTIVE && _vGameLevels[_currentLevel]->IsCompleted()) {
        GameOver();
        _pPostProcessor->Chaos = GL_TRUE;
        _state = GAME_WIN;
    }
}

void Game::Render()
{
    _pPostProcessor->BeginRender();
    if (_state == GAME_ACTIVE || _state == GAME_MENU) {
        for (Sprite2D* sprite : _sprites) {
            sprite->Draw();
        }
        
        for (GameObject &powerUP : _powerUPs) {
            powerUP.Draw();
        }
        
        std::stringstream ss;
        ss << _Lives;
        _pTextRender->RenderText("Lives: " + ss.str(), 5.0f, 5.0f, 1.0f);
    }
    
    if (_state == GAME_MENU) {
        _pTextRender->RenderText("Press ENTER to start", 250.0f, _windowHeight/2, 1.0f);
        _pTextRender->RenderText("Press W or S to select level", 245.0f, _windowHeight/2 + 20.0f, 0.75f);
    }
    
    if (_state == GAME_WIN) {
        _pTextRender->RenderText("You WON!!", 320.0f, _windowHeight/2 - 20.0, 1.0, glm::vec3(0.0f, 1.0f, 0.0f));
        _pTextRender->RenderText("Press ENTER to retry or ESC to quit", 130.0f, _windowHeight/2, 1.0, glm::vec3(1.0f, 1.0f, 1.0));
    }

    _pPostProcessor->EndRender();

    _pPostProcessor->Render(glfwGetTime());
}

GLboolean Game::ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(GameObject &block)
{
    if (ShouldSpawn(75)) {
        _powerUPs.push_back(PowerUp(PT_SPEED, glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.GetPosition(), "Texture/powerup_speed.png", _pQuadRenderer));
    }
    if (ShouldSpawn(75)) {
        _powerUPs.push_back(PowerUp(PT_STICKY, glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.GetPosition(), "Texture/powerup_sticky.png", _pQuadRenderer));
    }
    if (ShouldSpawn(75)) {
        _powerUPs.push_back(PowerUp(PT_PASS_THROUGH, glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.GetPosition(), "Texture/powerup_passthrough.png", _pQuadRenderer));
    }
    if (ShouldSpawn(75)) {
        _powerUPs.push_back(PowerUp(PT_PAD_SIZE_INCREASE, glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.GetPosition(), "Texture/powerup_increase.png", _pQuadRenderer));
    }
    if (ShouldSpawn(15)) {
        _powerUPs.push_back(PowerUp(PT_CONFUSE, glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.GetPosition(), "Texture/powerup_confuse.png", _pQuadRenderer));
    }
    if (ShouldSpawn(15)) {
        _powerUPs.push_back(PowerUp(PT_CHAOS, glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.GetPosition(), "Texture/powerup_chaos.png", _pQuadRenderer));
    }
}

void Game::UpdatePowerUps(GLfloat dt)
{
    for (PowerUp &pow : _powerUPs) {
        GLfloat py = pow.GetPosition().y;
        py = py + pow.GetVelocity().y * dt;
        
        pow.SetPosition(pow.GetPosition().x, py);
        
        if (pow.GetActivated()) {
            pow.SetDuration(pow.GetDuration() - dt);
            
            if (pow.GetDuration() <= 0.0f) {
                pow.SetActiviated(GL_FALSE);
                
                PowerUpType put = pow.GetType();
                if (put == PT_STICKY && !IsOtherPowerUpActive(PT_STICKY)) {
                    _pBall->SetSticky(GL_FALSE);
                    _pPaddle->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                }
                else if (put == PT_PASS_THROUGH && !IsOtherPowerUpActive(PT_PASS_THROUGH)) {
                    _pBall->SetPassThrought(GL_FALSE);
                    _pPaddle->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                }
                else if (put == PT_CONFUSE && !IsOtherPowerUpActive(PT_CONFUSE)) {
                    _pPostProcessor->Confuse = GL_FALSE;
                }
                else if (put == PT_CHAOS && !IsOtherPowerUpActive(PT_CHAOS)) {
                    _pPostProcessor->Chaos = GL_FALSE;
                }
            }
        }
    }
    
    _powerUPs.erase(std::remove_if(_powerUPs.begin(), _powerUPs.end(), [](const PowerUp &powerup) {
        return powerup.IsDestroyed() && !powerup.GetActivated();
    }), _powerUPs.end());
}

void Game::ActivatePowUp(PowerUp &powerup)
{
    if (powerup.GetType() == PT_SPEED) {
        _pBall->SetVelocity(_pBall->GetVelocity() * 1.2f);
    }
    else if (powerup.GetType() == PT_STICKY) {
        _pBall->SetSticky(GL_TRUE);
        _pPaddle->SetColor(1.0f, 0.5f, 1.0f, 1.0f);
    }
    else if (powerup.GetType() == PT_PASS_THROUGH) {
        _pBall->SetPassThrought(GL_TRUE);
        _pBall->SetColor(1.0f, 0.5f, 0.6f, 1.0f);
    }
    else if (powerup.GetType() == PT_PAD_SIZE_INCREASE) {
        _pPaddle->SetSize(_pPaddle->GetWidth() + 50, _pPaddle->GetHeight());
    }
    else if (powerup.GetType() == PT_CONFUSE) {
        if (!_pPostProcessor->Chaos) {
            _pPostProcessor->Confuse = GL_TRUE;
        }
    }
    else if (powerup.GetType() == PT_CHAOS) {
        if (!_pPostProcessor->Confuse) {
            _pPostProcessor->Chaos = GL_TRUE;
        }
    }
}

GLboolean Game::IsOtherPowerUpActive(PowerUpType type)
{
    for (PowerUp &powerUp : _powerUPs)
    {
        if (powerUp.GetActivated() && powerUp.GetType() == type) {
            GL_TRUE;
        }
    }
    return GL_FALSE;
}
