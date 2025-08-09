#include "UIDebug.hpp"

#include "imgui.h"

UIDebug::UIDebug() {
}

UIDebug::~UIDebug() {
}

void UIDebug::content() {
    ImGui::Begin("Debug Info");
    ImGui::Text("UI debug information");
    ImGui::End();
}
