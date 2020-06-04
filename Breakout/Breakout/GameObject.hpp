//
//  GameObject.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include "Sprite2D.hpp"

class GameObject : public Sprite2D
{
public:
    GameObject(std::string file, Renderer *renderer);

    ~GameObject();
    
    void Update(GLfloat dt);
    
    GLboolean IsSolid() const {
        return _bSolid;
    }
    
    GLboolean IsDestroyed() const {
        return _bDestroyed;;
    }
    
    void SetSolid(GLboolean value) {
        _bSolid = value;
    }
    
    void SetDestroyed(GLboolean value) {
        _bDestroyed = value;
    }
    
private:
    
    glm::vec2       _velocity;
    
    GLboolean       _bSolid;
    
    GLboolean       _bDestroyed;
};

#endif /* GameObject_hpp */
