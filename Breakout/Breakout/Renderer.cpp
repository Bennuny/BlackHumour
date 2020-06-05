//
//  SpriteRenderer.cpp
//  Breakout
//
//  Created by Ben on 2020/6/2.
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
    glad_glDeleteVertexArrays(1, &_quadVAO);
}

void Renderer::DrawSprite(Texture2D &texture, glm::vec2 pos, glm::vec2 size, GLfloat rotate, glm::vec4 color)
{
    _shader.Use();
    
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    
    if (rotate != 0) {
        model = glm::translate(model, glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f));
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f));
    }
    
    model = glm::scale(model, glm::vec3(size, 1.0f));

    _shader.SetMatrix4("model", model);
    _shader.SetVector4f("imageColor", color);
    
    glad_glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    
    glad_glBindVertexArray(_quadVAO);
    glad_glDrawArrays(GL_TRIANGLES, 0, 6);
    glad_glBindVertexArray(0);
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
