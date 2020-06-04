//
//  GameLevel.hpp
//  Breakout
//
//  Created by Ben on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include "GameObject.hpp"

#include "Renderer.hpp"

#include <vector>

class GameLevel
{
public:
    GameLevel();
    
    void Load(const GLchar *file, Renderer* pRenderer, GLuint levelWidth, GLuint levelHeight);
    
    void Draw();
    
    GLboolean IsCompleted();

private:
    std::vector<GameObject>     _vBricks;
    
};

#endif /* GameLevel_hpp */
