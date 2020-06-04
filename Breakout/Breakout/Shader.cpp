//
//  Shader.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader():
    _ID(0)
{
    
}

Shader::~Shader()
{
    
}
    
Shader& Shader::Use()
{
    glad_glUseProgram(_ID);
    return *this;
}
    
void Shader::Compile(const GLchar *vertexCode, const GLchar *fragmentCode, const GLchar *geometryCode)
{
    GLuint sVerter = createShader(vertexCode, "V");
    
    GLuint sFragment = createShader(fragmentCode, "F");
    
    GLuint sGeometry = geometryCode == nullptr ? 0 : createShader(geometryCode, "G");

    _ID = glad_glCreateProgram();
    
    glad_glAttachShader(_ID, sVerter);
    glad_glAttachShader(_ID, sFragment);
    if (geometryCode) {
        glad_glAttachShader(_ID, sGeometry);
    }
    
    glad_glLinkProgram(_ID);
    checkCompileErrors(_ID, "PROGRAM");
    
    glad_glDeleteShader(sVerter);
    glad_glDeleteShader(sFragment);
    if (geometryCode) {
        glad_glDeleteShader(sGeometry);
    }
}

GLuint Shader::createShader(const GLchar *sourceCode, std::string type)
{
    GLuint shader_o = 0;

    if (type == "V") {  // vertex
        shader_o = glad_glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == "F") { // fragment
        shader_o = glad_glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if (type == "G") { // geometry
        shader_o = glad_glCreateShader(GL_GEOMETRY_SHADER);
    }
    
    glad_glShaderSource(shader_o, 1, &sourceCode, nullptr);
    
    glad_glCompileShader(shader_o);
    
    checkCompileErrors(shader_o, "SHADER");
    
    return shader_o;
}


// SET UNIFORM FUNCTION
void Shader::SetFloat(const GLchar *name, GLfloat value)
{
    glad_glUniform1f(glad_glGetUniformLocation(_ID, name), value);
}

void Shader::SetInteger(const GLchar *name, GLint value)
{
    glad_glUniform1i(glad_glGetUniformLocation(_ID, name), value);
}

void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y)
{
    glad_glUniform2f(glad_glGetUniformLocation(_ID, name), x, y);
}
void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value)
{
    SetVector2f(name, value.x, value.y);
}

void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
{
    glad_glUniform3f(glad_glGetUniformLocation(_ID, name), x, y, z);
}
void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value)
{
    SetVector3f(name, value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glad_glUniform4f(glad_glGetUniformLocation(_ID, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value)
{
    SetVector4f(name, value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &value)
{
    glad_glUniformMatrix4fv(glad_glGetUniformLocation(_ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type == "PROGRAM") {
        glad_glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glad_glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Link-Time error-type: " << type << "\n" << infoLog << "\n  -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glad_glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glad_glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-Time error: Type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
