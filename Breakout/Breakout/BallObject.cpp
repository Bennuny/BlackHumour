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

void BallObject::Move(GLfloat dt)
{
    if (_bStuck) {
        return;
    }

    glm::vec2 offset = _velocity * dt;

    _position += offset;

    if (_position.x < 0.0f) {
        _position.x = 0.0f;
        _velocity.x = -_velocity.x;
    }
    else if (_position.x > _maxX) {
        _position.x = _maxX;
        _velocity.x = -_velocity.x;
    }
    
    if (_position.y < 0.0f) {
        _position.y = 0.0f;
        _velocity.y = -_velocity.y;
    }
}
