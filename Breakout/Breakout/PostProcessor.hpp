//
//  PostProcessor.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/5.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef PostProcessor_hpp
#define PostProcessor_hpp

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

#include "Texture2D.hpp"

class PostProcessor {

public:
    
    GLboolean   Confuse;
    GLboolean   Chaos;
    GLboolean   Shake;
    
    PostProcessor(Shader shader, GLuint width, GLuint height);
    
    void BeginRender();
    
    void EndRender();
    
    void Render(GLfloat dt);
    

private:
    
    Texture2D  _texture;
    
    Shader  _postProcessingShader;
    
    GLuint  MSFBO;
    
    GLuint  FBO;
    
    GLuint  RBO;
    
    GLuint  VAO;

    void    initRenderData();
    
    GLuint  _width;
    GLuint  _height;
};

#endif /* PostProcessor_hpp */
