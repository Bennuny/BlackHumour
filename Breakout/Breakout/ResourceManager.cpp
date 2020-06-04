//
//  ResourceManager.cpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "ResourceManager.hpp"

#include <sstream>
#include <fstream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
#endif

std::map<std::string, Shader>       ResourceManager::_shaders;

std::map<std::string, Texture2D>    ResourceManager::_textures;

std::string ResourceManager::SHADER_MODEL_TEX_COLOR = "MODEL_TEX_COLOR";

Shader ResourceManager::GetShader(std::string name)
{
    if (_shaders.find(name) == _shaders.end()) {
        loadShader(name);
    }
    
    return _shaders[name];
}

Texture2D ResourceManager::GetTexture2D(std::string name)
{
    if (_textures.find(name) == _textures.end()) {
        loadTexture2D(name, name);
    }
    return _textures[name];
}

void ResourceManager::clear()
{
    for (auto sd : _shaders) {
        glad_glDeleteProgram(sd.second.GetID());
    }
    for (auto tit : _textures) {
        GLuint id = tit.second.GetID();
        glad_glDeleteTextures(1, &id);
    }
}

std::string ResourceManager::GetFullPath(std::string file)
{
    #ifdef __APPLE__
        return "/Users/ben/work/1-Bennuny/LearnOpenGL/Breakout/Resources/" + file;
    #endif
    
    return file;
}

void ResourceManager::loadShader(std::string name)
{
    if (name == SHADER_MODEL_TEX_COLOR) {
        Shader shader = loadShaderFromFile("Shader/model_tex_color.vert", "Shader/model_tex_color.frag");
        _shaders.insert(std::pair<std::string, Shader>(SHADER_MODEL_TEX_COLOR, shader));
    }
}

Texture2D ResourceManager::loadTexture2D(std::string file, std::string name)
{
    Texture2D texture = LoadTextureFromFile(file.c_str());
    _textures.insert(std::pair<std::string, Texture2D>(name, texture));
    
    return texture;
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vfile, const GLchar *ffile, const GLchar *gfile)
{
    Shader shader;

    std::string vert = getStringFromFile(vfile);
    std::string frag = getStringFromFile(ffile);
    if (gfile) {
        std::string geom = getStringFromFile(gfile);
        shader.Compile(vert.c_str(), frag.c_str(), geom.c_str());
    }
    else {
        shader.Compile(vert.c_str(), frag.c_str());
    }
    
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const GLchar *file)
{
    std::string fullpath = GetFullPath(file);

//    stbi_set_flip_vertically_on_load(true);

    Texture2D texture;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(fullpath.c_str(), &width, &height, &nrComponents, 0);
        
    if (data)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1) {
            format = GL_RED;
        }
        else if (nrComponents == 3) {
            format = GL_RGB;
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        texture.SetInternalFormat(format);
        texture.SetImageFormat(format);

        texture.Generate(width, height, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(data);
    }

    return texture;
}

std::string ResourceManager::getStringFromFile(const GLchar *file)
{
    std::string fullpath = GetFullPath(file);
    
    std::string sourceCode;
    
    try {
        std::ifstream ifs(fullpath);
        
        std::stringstream ss;
        ss << ifs.rdbuf();
        
        ifs.close();
        
        sourceCode = ss.str();
        
    } catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read files: " << file << std::endl;
    }
    
    return sourceCode;
}
