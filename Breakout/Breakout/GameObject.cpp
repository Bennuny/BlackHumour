//
//  GameObject.cpp
//  Breakout
//
//  Created by Ben on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameObject.hpp"

GameObject::GameObject(std::string file, Renderer *renderer):
    Sprite2D(file, renderer),
    _velocity(0.0f),
    _bSolid(GL_FALSE),
    _bDestroyed(GL_FALSE)
{

}

GameObject::~GameObject()
{
    
}

void GameObject::Update(GLfloat dt)
{
    const glm::vec2 pos = GetPosition();
    SetPosition(pos.x + _velocity.x, pos.y + _velocity.y);
}
