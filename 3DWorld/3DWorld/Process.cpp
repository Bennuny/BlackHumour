//
//  Process.cpp
//  3DWorld
//
//  Created by 付备 on 2020/6/8.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "imgui.h"

#include <iostream>



static void im_showFrame()
{
    ImGui::Begin("Frame");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SetWindowPos(ImVec2(1280 - ImGui::GetWindowWidth(), 720 - ImGui::GetWindowHeight()));
    ImGui::End();
}
