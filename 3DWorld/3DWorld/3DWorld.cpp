//
//  3DWorld.cpp
//  3DWorld
//
//  Created by 付备 on 2020/6/8.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "3DWorld.hpp"

#include "imgui.h"

ThreeDWorld::ThreeDWorld(unsigned int w, unsigned int h):
    _windowWidth(w),
    _windowHeight(h)
{
    
}

ThreeDWorld::~ThreeDWorld()
{
    
}

void ThreeDWorld::Render()
{
    ImGui::Begin("Frame");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SetWindowPos(ImVec2(_windowWidth - ImGui::GetWindowWidth(), _windowHeight - ImGui::GetWindowHeight()));
    ImGui::End();
}
