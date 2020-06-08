//
//  PowerUp.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/7.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "PowerUp.hpp"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

PowerUp::PowerUp(PowerUpType type, glm::vec3 color, GLfloat duration, glm::vec2 position, std::string file, Renderer *pRenderer) :
    GameObject(file, pRenderer),
    _type(type),
    _duration(duration)
{
    _color = glm::vec4(color, 1.0f);
    
    _position = position;
    
    SetSize(SIZE.x, SIZE.y);
    
    SetVelocity(VELOCITY);
}
