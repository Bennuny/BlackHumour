//
//  Particle.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/5.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.hpp"

#include "Node.hpp"

struct Particle
{
    glm::vec2   Position;
    glm::vec2   Velocity;
    
    glm::vec4   Color;
    
    GLfloat     Life;

    Particle(): Position(0.0f), Velocity(0.0f), Color(0.0f), Life(0.0f)
    {
        
    }
};

class ParticleManager : public Node {
public:
    ParticleManager(Renderer *renderer, std::string file, GLuint amount);
    
    void Update(GLfloat dt, GameObject *object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    
    virtual void Draw();
    
private:
    std::vector<Particle>   _vParticles;
    
    GLuint                  _amount;
    
    GLuint                  _lastUsedParticle;
    
    Texture2D               _texture;
    
    Renderer*               _pRenderer;

    void init();
    
    GLuint FirstUnusedParticle();
    
    void RespawnParticle(Particle &particle, GameObject *object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif /* Particle_hpp */
