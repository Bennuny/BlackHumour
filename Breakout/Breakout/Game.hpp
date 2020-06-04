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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_FAIE,
};

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
    
private:
    GameState       _state;
        
    Sprite2D*                _pPaddle;
    BallObject*              _pBall;
    std::vector<Sprite2D*>   _sprites;
  
    GLuint          _windowWidth;
    GLuint          _windowHeight;
    
    Renderer                    *_pQuadRenderer;
    std::vector<GameLevel* >    _vGameLevels;
    unsigned int                _currentLevel;
};


#endif /* Game_hpp */
