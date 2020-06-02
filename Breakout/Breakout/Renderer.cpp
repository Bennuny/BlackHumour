//
//  SpriteRenderer.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Renderer.hpp"

#include "ResourceManager.hpp"

Renderer::Renderer(Shader &shader):
    _shader(shader),
    _quadVAO(0)
{
    initRenderData();
}

Renderer::~Renderer()
{
    
}

void Renderer::DrawSprite(Texture2D &texture, glm::vec2 pos, glm::vec2 size, GLfloat rotate, glm::vec4 color)
{
    
}

void Renderer::initRenderData()
{
    GLuint VBO;
    GLfloat vertices[] = {
        // 位置     // 纹理
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glad_glGenVertexArrays(1, &_quadVAO);
    glad_glGenBuffers(1, &VBO);

    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glad_glBindVertexArray(_quadVAO);
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);

}
