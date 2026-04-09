#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPUApollonianGasket : public GPUFractalComponent {
protected:
    float m_colorFrequency = 2.0f;
    float m_colorOffset = 0.0f;
    float m_foldScale = 1.0f;
    float m_spread = 1.5f;

    void UploadExtraUniforms() override;

public:
    GPUApollonianGasket(std::string name);
    virtual ~GPUApollonianGasket() = default;

    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};