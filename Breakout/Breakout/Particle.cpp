//
//  Particle.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/5.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Particle.hpp"

#include "ResourceManager.hpp"

ParticleManager::ParticleManager(Renderer *renderer, std::string file, GLuint amount):
    _pRenderer(renderer),
    _amount(amount),
    _lastUsedParticle(0)
{
    _texture = ResourceManager::GetTexture2D(file);

    init();
}
    
void ParticleManager::Update(GLfloat dt, GameObject *object, GLuint newParticles, glm::vec2 offset)
{
    // Add New
    for (int i = 0; i < newParticles; i++) {
        int particleidx = FirstUnusedParticle();
        RespawnParticle(_vParticles[particleidx], object, offset);
    }

    // Update
    for (int i = 0; i < _amount; i++) {
        _vParticles[i].Life -= dt;

        if (_vParticles[i].Life > 0.0f) {

            _vParticles[i].Position -= _vParticles[i].Velocity * dt;
            _vParticles[i].Color -= dt * 2.5f;
        }
    }
}

void ParticleManager::Draw()
{
    glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    for (Particle& particle : _vParticles) {
        if (particle.Life > 0.0f) {
            _pRenderer->DrawParticle(_texture, particle.Position, particle.Color);
        }
    }
    
//    _pRenderer->DrawParticle(_texture, glm::vec2(400, 300), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleManager::init()
{
    for (int i = 0; i < _amount; i++) {
        _vParticles.push_back(Particle());
    }
}
    
GLuint ParticleManager::FirstUnusedParticle()
{
    for (int i = _lastUsedParticle; i < _amount; i++) {
        if (_vParticles[i].Life <= 0.0f) {
            _lastUsedParticle = i;
            return i;
        }
    }
    
    for (int i = 0; i < _lastUsedParticle; i++) {
        if (_vParticles[i].Life <= 0.0f) {
            _lastUsedParticle = i;
            return i;
        }
    }
    
    _lastUsedParticle = 0;
    return 0;
}
    
void ParticleManager::RespawnParticle(Particle &particle, GameObject *object, glm::vec2 offset)
{
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    
    particle.Position = object->GetPosition() + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object->GetVelocity() * 0.1f;
}
