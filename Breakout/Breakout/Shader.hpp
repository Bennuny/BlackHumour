//
//  Shader.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Shader
{
public:
    Shader();
    
    ~Shader();
    
    Shader& Use();
    
    void Compile(const GLchar *vertexCode, const GLchar *fragmentCode, const GLchar *geometryCode = nullptr);

    GLuint GetID() const {
        return _ID;
    }
    
    // SET UNIFORM FUNCTION
    void SetFloat(const GLchar *name, GLfloat value);
    
    void SetInteger(const GLchar *name, GLint value);
    
    void SetVector2f(const GLchar *name, GLfloat x, GLfloat y);
    void SetVector2f(const GLchar *name, const glm::vec2 &value);
    
    void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
    void SetVector3f(const GLchar *name, const glm::vec3 &value);
    
    void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void SetVector4f(const GLchar *name, const glm::vec4 &value);

    void SetMatrix4(const GLchar *name, const glm::mat4 &value);
    
private:
    GLuint  _ID;
    
    void checkCompileErrors(GLuint object, std::string type);
    
    GLuint createShader(const GLchar *sourceCode, std::string type);
};

#endif /* Shader_hpp */
