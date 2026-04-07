#pragma once
#include "../FractalComponent.hpp"

class MandelbrotComponent : public FractalComponent {
private:
    std::string m_name;
    int m_maxIterations = 100;

public:
    MandelbrotComponent(std::string name);

    void DrawControlPanel() override;
    std::string GetLabel() const override;
    void UpdateTexture() override;
};