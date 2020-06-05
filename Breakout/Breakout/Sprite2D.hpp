//
//  Sprite2D.hpp
//  Breakout
//
//  Created by Ben on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Sprite2D_hpp
#define Sprite2D_hpp

#include "Texture2D.hpp"

#include <iostream>

#include "Renderer.hpp"

#include "Node.hpp"

class Sprite2D : public Node
{
public:
    Sprite2D(std::string file, Renderer *renderer);

    void initWithFile(std::string file);
    
    virtual void Draw();

private:
    Texture2D        _texture;
    Renderer        *_pRenderer;
};

#endif /* Sprite2D_hpp */
