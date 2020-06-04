//
//  GameLevel.cpp
//  Breakout
//
//  Created by Ben on 2020/6/3.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "GameLevel.hpp"

#include "ResourceManager.hpp"

GameLevel::GameLevel()
{
    
}
    
void GameLevel::Load(const GLchar *file, Renderer* pRenderer, GLuint levelWidth, GLuint levelHeight)
{
    _vBricks.clear();

    std::vector<std::vector<int>> vTileData = ResourceManager::GetLevelFromPath(file);

    unsigned int height = static_cast<unsigned int>(vTileData.size());
    unsigned int width = static_cast<unsigned int>(vTileData[0].size());
    
    float unit_width = levelWidth / static_cast<GLfloat>(width);
    float unit_height = levelHeight / static_cast<GLfloat>(height);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            if (vTileData[y][x] == 1) {
                GameObject object("Texture/block_solid.png", pRenderer);
                
                object.SetPosition(unit_width * x, unit_height * y);
                object.SetSize(unit_width, unit_height);
                object.SetColor(0.8f, 0.8f, 0.7f, 1.0f);
                object.SetSolid(GL_TRUE);
                
                _vBricks.push_back(object);
            }
            else if (vTileData[y][x] > 0) {
                GameObject object("Texture/block.png", pRenderer);
                object.SetPosition(unit_width * x, unit_height * y);
                object.SetSize(unit_width, unit_height);
                object.SetSolid(GL_FALSE);

                if (vTileData[y][x] == 2) {
                    object.SetColor(0.2f, 0.6f, 1.0f, 1.0f);
                }
                else if (vTileData[y][x] == 3) {
                    object.SetColor(0.0f, 0.7f, 0.0f, 1.0f);
                }
                else if (vTileData[y][x] == 4) {
                    object.SetColor(0.8f, 0.8f, 0.4f, 1.0f);
                }
                else if (vTileData[y][x] == 5) {
                    object.SetColor(1.0f, 0.5f, 0.0f, 1.0f);
                }
                _vBricks.push_back(object);
            }
        }
    }
}
    
void GameLevel::Draw()
{
    for (GameObject object : _vBricks) {
        object.Draw();
    }
}
    
GLboolean GameLevel::IsCompleted()
{
    return GL_FALSE;
}
