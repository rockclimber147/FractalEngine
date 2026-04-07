#pragma once
#include "../FractalComponent.hpp"

class MandelbrotComponent : public FractalComponent {
private:
    std::string m_name;
    int m_maxIterations = 100;
    float m_colorFrequency = 20.0f;
    float m_colorOffset = 0.0f;

    void MapColor(double escapeVal, unsigned char* rgbOut);

public:
    MandelbrotComponent(std::string name);

    void DrawControlPanel() override;
    std::string GetLabel() const override;
    void UpdateTexture() override;
};