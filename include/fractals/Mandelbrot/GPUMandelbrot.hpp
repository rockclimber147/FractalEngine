#pragma once

#include "fractals/GPUFractalComponent.hpp"
#include <string>

class GPUMandelbrot : public GPUFractalComponent {
private:
    std::string m_name;
    int m_maxIterations = 500;
    float m_colorFrequency = 1.0f;
    float m_colorOffset = 0.0f;

protected:
    const char* GetFragmentShaderSource() override;
    void UploadExtraUniforms() override;

public:
    GPUMandelbrot(std::string name);
    virtual ~GPUMandelbrot() = default;

    // ImGui Interface
    std::string GetLabel() const override;
    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};