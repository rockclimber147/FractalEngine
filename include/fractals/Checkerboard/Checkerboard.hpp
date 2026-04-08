#pragma once
#include "fractals/CPUFractalComponent.hpp"

class Checkerboard : public CPUFractalComponent {
private:
    std::string m_name;
    float m_brightness = 1.0f;
    std::string m_imguiID;

public:
    Checkerboard(std::string name);

    // ControlComponent Implementations
    void DrawControlPanel() override;
    std::string GetLabel() const override;

    void GetPixelColor(double u, double v, unsigned char rgb[3]) override;
};