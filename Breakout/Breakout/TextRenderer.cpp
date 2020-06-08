//
//  TextRenderer.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/7.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "TextRenderer.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(Shader shader, glm::mat4& projection)
{
    _textShader = shader;
    
    _textShader.Use();
    _textShader.SetMatrix4("projection", projection);

    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    
    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, GLuint fontsize)
{
    _mapCharacter.clear();
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    
    FT_Set_Pixel_Sizes(face, 0, fontsize);
    
    glad_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph " << std::endl;
            continue;
        }
        
        GLuint texture;
        glad_glGenTextures(1, &texture);
        glad_glBindTexture(GL_TEXTURE_2D, texture);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        _mapCharacter.insert(std::pair<GLchar, Character>(c, character));
    }
    
    glad_glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    _textShader.Use();
    _textShader.SetVector3f("color", color);
    glad_glActiveTexture(GL_TEXTURE0);
    glad_glBindVertexArray(VAO);
    
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = _mapCharacter[*c];
        
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y + (_mapCharacter['H'].Bearing.y - ch.Bearing.y) * scale;
        
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        
        GLfloat vertices[6][4] = {
            {xpos,       ypos+h,     0.0f, 1.0f},
            {xpos+w,     ypos,       1.0f, 0.0f},
            {xpos,       ypos,       0.0f, 0.0f},
            
            {xpos,      ypos+h,     0.0f, 1.0f},
            {xpos+w,    ypos+h,     1.0f, 1.0f},
            {xpos+w,    ypos,       1.0f, 0.0f}
        };
        glad_glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        
        glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glad_glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
        glad_glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (ch.Advance >> 6) * scale;
    }
    
    glad_glBindVertexArray(0);
    glad_glBindTexture(GL_TEXTURE_2D, 0);
}
