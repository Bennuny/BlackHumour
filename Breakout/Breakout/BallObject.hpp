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
    
    GLboolean isSticky() const {
        return _bSticky;
    }
    
    void SetSticky(GLboolean value) {
        _bSticky = value;
    }
    
    GLboolean PassThrough() const {
        return _bPassThrough;
    }
    
    void SetPassThrought(GLboolean value) {
        _bPassThrough = value;
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
    
    void Move(GLfloat dt);
    
private:
    
    GLboolean       _bStuck;
    
    GLfloat         _radius;
    
    GLfloat         _maxX;
    
    GLboolean       _bPassThrough;
    
    GLboolean       _bSticky;
};

#endif /* BallObject_hpp */
