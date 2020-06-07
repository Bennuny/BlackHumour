//
//  PowerUp.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/7.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef PowerUp_hpp
#define PowerUp_hpp

#include "GameObject.hpp"

#include "Renderer.hpp"

enum PowerUpType {
    PT_SPEED,
    PT_STICKY,
    PT_PASS_THROUGH,
    PT_PAD_SIZE_INCREASE,
    PT_CONFUSE,
    PT_CHAOS
};

class PowerUp : public GameObject
{
public:
    
    PowerUp(PowerUpType type, glm::vec3 color, GLfloat duration, glm::vec2 position, std::string file, Renderer *pRenderer);
    
    
    PowerUpType GetType() const {
        return _type;
    }
    
    GLfloat GetDuration() {
        return _duration;
    }
    
    void SetDuration(GLfloat value) {
        _duration = value;
    }
    
    GLboolean GetActivated() const {
        return _activated;
    }
    
    void SetActiviated(GLboolean value) {
        _activated = value;
    }

private:
    
    PowerUpType _type;
    
    GLfloat     _duration;
    
    GLboolean   _activated;
};

#endif /* PowerUp_hpp */
