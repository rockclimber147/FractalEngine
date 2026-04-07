#pragma once
#include <string>
#include "../external/imgui/imgui.h"

class Component {
public:
    virtual ~Component() {}
    virtual void DrawControlPanel() = 0;
    virtual std::string GetLabel() const = 0;

    virtual ImTextureID GetResultTexture() const = 0;
    virtual void Pan(float dx, float dy, float vW, float vH) = 0;
    virtual void Zoom(float amount) = 0;
    virtual void Resize(int w, int h) = 0;
};