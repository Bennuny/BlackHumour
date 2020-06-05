//
//  BallObject.hpp
//  Breakout
//
//  Created by Ben on 2020/6/4.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef BallObject_hpp
#define BallObject_hpp

#include "GameObject.hpp"

class BallObject : public GameObject
{
public:
    BallObject(std::string file, Renderer *renderer);
    
    ~BallObject();

    void Reset(glm::vec2 pos, glm::vec2 velocity);

    GLboolean IsStuck() const {
        return _bStuck;
    }
    
    void SetStuck(GLboolean v) {
        _bStuck = v;
    }
    
    void SetRadius(GLfloat radius) {
        _radius = radius;
        
        _size.x = radius * 2;
        _size.y = radius * 2;
    }
    
    GLfloat GetRadius() const {
        return _radius;
    }
    
    void SetMaxX(GLfloat maxX) {
        _maxX = maxX;
    }
    
    void ReverseVelocityX() {
        _velocity.x = -_velocity.x;
    }
    
    void ReverseVelocityY() {
        _velocity.y = -_velocity.y;
    }

    glm::vec2 GetVelocity() const {
        return _velocity;
    }
    
    void SetVelocity(glm::vec2 vel) {
        _velocity = vel;
    }
    
    void Move(GLfloat dt);
    
private:
    
    GLboolean       _bStuck;
    
    GLfloat         _radius;
    
    GLfloat         _maxX;
};

#endif /* BallObject_hpp */
