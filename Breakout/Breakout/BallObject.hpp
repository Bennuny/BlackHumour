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
    
private:
    
    GLboolean       _bStuck;
    
    GLfloat         _radius;
};

#endif /* BallObject_hpp */
