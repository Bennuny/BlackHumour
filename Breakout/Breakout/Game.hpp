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

#include "Renderer.hpp"

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
    GameState       _state;
    
    std::vector<Sprite2D>   _sprites;
  
    GLuint          _windowWidth;
    GLuint          _windowHeight;
    
    Renderer                *_pQuadRenderer;
};


#endif /* Game_hpp */
