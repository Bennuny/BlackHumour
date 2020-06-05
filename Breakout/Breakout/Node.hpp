//
//  Node.h
//  Breakout
//
//  Created by 付备 on 2020/6/5.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Node
{
public:
    Node();

    virtual void Draw() {
        for (Node* node : _children) {
            node->Draw();
        }
    };

    void SetPosition(GLfloat x, GLfloat y) {
        _position.x = x;
        _position.y = y;
    }
    
    void SetPosition(glm::vec2 pos) {
        SetPosition(pos.x, pos.y);
    }
    
    const glm::vec2 GetPosition() const {
        return _position;
    }
    
    void SetSize(GLfloat w, GLfloat h) {
        _size.x = w;
        _size.y = h;
    }
    
    GLfloat GetWidth() const {
        return _size.x;
    }
    
    GLfloat GetHeight() const {
        return _size.y;
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
    
    void AddChild(Node *pnode) {
        _children.push_back(pnode);
    }
    
    void ClearAllChildren(GLboolean destroy = GL_FALSE) {
        if (destroy) {
            
        }
        
        _children.clear();
    }
    
protected:
    glm::vec2           _position;
    glm::vec2           _size;
    GLfloat             _rotate;
    glm::vec4           _color;

        
    std::vector<Node*>   _children;
};


#endif /* Node_h */
