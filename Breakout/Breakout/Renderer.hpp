//
//  SpriteRenderer.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef SpriteRenderer_hpp
#define SpriteRenderer_hpp

#include "Shader.hpp"

#include "Texture2D.hpp"

class Renderer
{
public:
    Renderer(Shader &shader);
    
    ~Renderer();
    
    void DrawSprite(Texture2D &texture, glm::vec2 pos, glm::vec2 size, GLfloat rotate, glm::vec4 color);

private:
    Shader  _shader;

    GLuint  _quadVAO;

    void initRenderData();
};

#endif /* SpriteRenderer_hpp */
