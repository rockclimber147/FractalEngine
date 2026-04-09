#pragma once

#include "fractals/GPUFractalComponent.hpp"
#include <string>
#include <vector>

struct JuliaPreset {
    std::string name;
    float real;
    float imag;
};


class GPUJulia : public GPUFractalComponent {
private:
    std::string m_name;
    int m_selectedPreset = 0;
    std::vector<JuliaPreset> m_presets;

    float juliaCReal = 0.0;
    float juliaCImag = 0.641;

    int m_maxIterations = 500;
    float m_colorFrequency = 1.0f;
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