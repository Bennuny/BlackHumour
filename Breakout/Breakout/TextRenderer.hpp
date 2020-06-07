//
//  TextRenderer.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/7.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef TextRenderer_hpp
#define TextRenderer_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Texture2D.hpp"
#include "Shader.hpp"

#include <map>


struct Character {
    GLuint          TextureID;
        
    glm::vec2       Size;
    glm::vec2       Bearing;
    
    GLuint          Advance;
};


class TextRenderer {
    
public:
    
    TextRenderer(Shader shader, glm::mat4& projection);
    
    void Load(std::string font, GLuint fontsize);
    
    void RenderText(std::string, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
    

private:
    std::map<GLchar, Character>     _mapCharacter;
    
    Shader  _textShader;

    GLuint  VAO;
    
    GLuint  VBO;
};

#endif /* TextRenderer_hpp */
