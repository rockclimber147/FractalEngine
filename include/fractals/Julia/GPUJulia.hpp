#pragma once

#include "fractals/GPUFractalComponent.hpp"
#include <string>

class GPUJulia : public GPUFractalComponent {
private:
    std::string m_name;

    float juliaCReal = 0.0;
    float juliaCImag = 0.641;

    int m_maxIterations = 500;
    float m_colorFrequency = 20.0f;
    float m_colorOffset = 0.0f;

protected:
    const char* GetFragmentShaderSource() override;
    void UploadExtraUniforms() override;

public:
    GPUJulia(std::string name);
    virtual ~GPUJulia() = default;

    // ImGui Interface
    std::string GetLabel() const override;
    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};