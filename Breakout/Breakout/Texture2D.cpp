//
//  Texture2D.cpp
//  Breakout
//
//  Created by Ben on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Texture2D.hpp"

#include <GLFW/glfw3.h>

Texture2D::Texture2D():
    _width(0),
    _height(0),
    _internal_format(GL_RGB),
    _image_format(GL_RGB),
    _wrap_s(GL_REPEAT),
    _wrap_t(GL_REPEAT),
    _filter_min(GL_LINEAR),
    _filter_max(GL_LINEAR)
{
    glad_glGenTextures(1, &_ID);
}
    
void Texture2D::Generate(GLuint width, GLuint height, unsigned char *data)
{
    _width = width;
    _height = height;
    
    glad_glBindTexture(GL_TEXTURE_2D, _ID);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, width, height, 0, _image_format, GL_UNSIGNED_BYTE, data);
    
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_s);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter_min);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter_max);
    
    glad_glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind()
{
    glad_glBindTexture(GL_TEXTURE_2D, _ID);
}
