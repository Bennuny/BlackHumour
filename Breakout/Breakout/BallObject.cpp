//
//  BallObject.cpp
//  Breakout
//
//  Created by Ben on 2020/6/4.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "BallObject.hpp"

BallObject::BallObject(std::string file, Renderer *renderer):
    GameObject(file, renderer),
    _maxX(0)
{
    
}

BallObject::~BallObject()
{
    
}

void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity)
{
    SetPosition(pos.x, pos.y);
    _velocity = velocity;
}

