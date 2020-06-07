//
//  Game.hpp
//  Breakout
//
//  Created by Ben on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "Sprite2D.hpp"
#include "BallObject.hpp"

#include "Renderer.hpp"

#include "GameLevel.hpp"

#include "PostProcessor.hpp"

#include "Particle.hpp"

#include "PowerUp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_FAIE,
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    DIR_COUNT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game
{
public:
    Game(GLuint width, GLuint height);
    
    ~Game();
    
    void Init();

    void ProcessInput(GLfloat dt);
    
    void Update(GLfloat dt);
    
    void Render();

    GameState GetState() const {
        return _state;
    }
    
    void SetState(GameState gs) {
        _state = gs;
    }

    GLboolean       Keys[1024];

private:
    Sprite2D* CreateSprite(std::string file);
    
    BallObject* CreateBallObject(std::string file);
    
    void ResetBall();
    void ResetPlayer();
    
    void ResetLevel();

    void GameOver();
    
    void SetLevel(unsigned int levelIdx);
    
    void DoCollision();
    
    // AABB (Axis-aligned Bounding Box) 轴对齐碰撞箱
    GLboolean CheckCollisionAABB(Node &one, Node &two);
    
    Collision CheckCollisionAABBCycle(BallObject &ball, Node &brick);
    
    Direction VectorDirection(glm::vec2 traget);
    
    GLboolean ShouldSpawn(GLuint chance);
    
    void SpawnPowerUps(GameObject &block);
    
    void UpdatePowerUps(GLfloat dt);
    
    void ActivatePowUp(PowerUp &powerup);
    
    GLboolean IsOtherPowerUpActive(PowerUpType type);
    
private:
    GameState       _state;
        
    Sprite2D*                _pBg;
    Sprite2D*                _pPaddle;
    BallObject*              _pBall;
    std::vector<Sprite2D*>   _sprites;
  
    GLuint          _windowWidth;
    GLuint          _windowHeight;
    
    Renderer                    *_pQuadRenderer;
    Renderer                    *_pParticleRenderer;
    
    std::vector<GameLevel* >    _vGameLevels;
    unsigned int                _currentLevel;
    
    ParticleManager             *_pParticleManager;
    
    PostProcessor               *_pPostProcessor;
    
    GLfloat                     _ShakeTime;
    
    std::vector<PowerUp>        _powerUPs;
};


#endif /* Game_hpp */
