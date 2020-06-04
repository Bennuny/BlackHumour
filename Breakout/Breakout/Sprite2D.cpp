//
//  Sprite2D.cpp
//  Breakout
//
//  Created by Ben on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Sprite2D.hpp"

#include "ResourceManager.hpp"

Sprite2D::Sprite2D(std::string file, Renderer *renderer):
    _position(0.0f),
    _size(1.0f),
    _rotate(0.0f),
    _color(1.0f),
    _pRenderer(renderer)
{
    initWithFile(file);
}

void Sprite2D::initWithFile(std::string file)
{
    _texture = ResourceManager::GetTexture2D(file);
    
    _size.x = _texture.GetWidth();
    _size.y = _texture.GetHeight();

    _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sprite2D::Draw()
{
    _pRenderer->DrawSprite(_texture, _position, _size, _rotate, _color);
}
