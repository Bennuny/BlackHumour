//
//  PostProcessor.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/5.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "PostProcessor.hpp"


PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height):
    Confuse(GL_FALSE),
    Chaos(GL_FALSE),
    Shake(GL_FALSE),
    MSFBO(0),
    FBO(0),
    VAO(0),
    _texture(),
    _width(width),
    _height(height),
    _postProcessingShader(shader)
{
    Confuse = GL_FALSE;
    Chaos = GL_FALSE;
    Shake = GL_FALSE;
    
    glad_glGenFramebuffers(1, &MSFBO);
    glad_glGenFramebuffers(1, &FBO);
    glad_glGenRenderbuffers(1, &RBO);
    
    glad_glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glad_glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // Allocate storage for render buffe object
    glad_glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
    //    Attach MS render buffer object to framebuffer
    glad_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
    if (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
    }


    glad_glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    _texture.Generate(width, height, nullptr);
    glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.GetID(), 0);
    if (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    }
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initRenderData();
 
    shader.Use();
    shader.SetInteger("scene", 0);
    
    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    glad_glUniform2fv(glad_glGetUniformLocation(_postProcessingShader.GetID(), "offsets"), 9, (GLfloat*)offsets);
    GLint edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glad_glUniform1iv(glad_glGetUniformLocation(_postProcessingShader.GetID(), "edge_kernel"), 9, edge_kernel);
    GLfloat blur_kernel[9] = {
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    };
    glad_glUniform1fv(glad_glGetUniformLocation(_postProcessingShader.GetID(), "blur_kernel"), 9, blur_kernel);
}


void PostProcessor::initRenderData()
{
    GLuint VBO;
    
    GLfloat vertices[] = {
        -1.0f, -1.0f,   0.0f, 0.0f,
        1.0f,   1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,

        -1.0f,  -1.0f,  0.0f, 0.0f,
        1.0f,   -1.0f,  1.0f, 0.0f,
        1.0f,    1.0f,   1.0f, 1.0f
    };
    
    glad_glGenBuffers(1, &VBO);
    glad_glGenVertexArrays(1, &VAO);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBindVertexArray(VAO);
    
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);
    
}

void PostProcessor::BeginRender()
{
    glad_glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
    glad_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glad_glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
    // Now resolve multisampled color-buffer into intermediate FBO to store to texture
    glad_glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
    glad_glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glad_glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0); // Binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::Render(GLfloat time)
{
    // Set uniforms/options
    this->_postProcessingShader.Use();
    this->_postProcessingShader.SetFloat("time", time);
    
    this->_postProcessingShader.SetInteger("confuse", Confuse);
    this->_postProcessingShader.SetInteger("chaos", Chaos);
    this->_postProcessingShader.SetInteger("shake", Shake);

    // Render textured quad
    glad_glActiveTexture(GL_TEXTURE0);
    _texture.Bind();
    glad_glBindVertexArray(VAO);
    glad_glDrawArrays(GL_TRIANGLES, 0, 6);
    glad_glBindVertexArray(0);
}
