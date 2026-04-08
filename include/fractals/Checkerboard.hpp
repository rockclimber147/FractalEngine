#pragma once
#include "../FractalComponent.hpp"

class Checkerboard : public FractalComponent {
private:
    std::string m_name;
    float m_brightness = 1.0f;
    std::string m_imguiID;

public:
    Checkerboard(std::string name);

    // ControlComponent Implementations
    void DrawControlPanel() override;
    std::string GetLabel() const override;

    // FractalComponent Implementation
    void UpdateTexture() override;
};