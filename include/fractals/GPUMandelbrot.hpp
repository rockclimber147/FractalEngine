#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPUMandelbrot : public GPUFractalComponent {
private:
    std::string m_name;
    int m_maxIterations = 500;
    float m_colorFrequency = 1.0f;
    float m_colorOffset = 0.0f;

protected:
    void UploadExtraUniforms() override;

public:
    GPUMandelbrot(std::string name);
    virtual ~GPUMandelbrot() = default;

    // ImGui Interface
    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};