//
//  ResourceManager.hpp
//  Breakout
//
//  Created by 付备 on 2020/6/2.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include "Texture2D.hpp"
#include "Shader.hpp"

#include <map>
#include <iostream>

class ResourceManager
{
public:
    // 位置-纹理-颜色
    static std::string SHADER_MODEL_TEX_COLOR;
    
public:
    static Shader GetShader(std::string name);
    
    static Texture2D GetTexture2D(std::string name);

    static void clear();

    static std::string GetFullPath(std::string file);
    
private:

    static  std::map<std::string, Shader>       _shaders;
    
    static  std::map<std::string, Texture2D>    _textures;
    
    static void loadShader(std::string name);
    
    static Texture2D loadTexture2D(std::string file, std::string name);

    static Shader loadShaderFromFile(const GLchar *vfile, const GLchar *ffile, const GLchar *gfile = nullptr);
    
    static Texture2D LoadTextureFromFile(const GLchar *file);
    
    static std::string getStringFromFile(const GLchar *file);
};


#endif /* ResourceManager_hpp */
