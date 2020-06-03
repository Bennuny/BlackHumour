//
//  Sprite2D.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Sprite2D_hpp
#define Sprite2D_hpp

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.hpp"

#include <iostream>

#include "Renderer.hpp"

class Sprite2D
{
public:
    Sprite2D(std::string file, Renderer *renderer);

    void initWithFile(std::string file);
    
    void Draw();

    void SetPosition(GLfloat x, GLfloat y) {
        _position.x = x;
        _position.y = y;
    }
    
    void SetSize(GLfloat w, GLfloat h) {
        _size.x = w;
        _size.y = h;
    }
    
    void SetRotate(GLfloat rotate) {
        _rotate = rotate;
    }
    
    void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        _color.r = r;
        _color.g = g;
        _color.b = b;
        _color.a = a;
    }
    
private:
    glm::vec2   _position;
    glm::vec2   _size;
    
    GLfloat     _rotate;

    glm::vec4   _color;
    
    Texture2D   _texture;
    
    Renderer    *_pRenderer;
};

#endif /* Sprite2D_hpp */
