#pragma once
#include "../FractalComponent.hpp"

class CheckerboardComponent : public FractalComponent {
private:
    std::string m_name;
    float m_brightness = 1.0f;
    std::string m_imguiID;

public:
    CheckerboardComponent(std::string name);

    // ControlComponent Implementations
    void DrawControlPanel() override;
    std::string GetLabel() const override;

    // FractalComponent Implementation
    void UpdateTexture() override;
};