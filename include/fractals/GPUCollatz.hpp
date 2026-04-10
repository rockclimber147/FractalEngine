#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPUCollatz : public GPUFractalComponent {
protected:
    float m_colorFrequency = 2.0f;
    float m_colorOffset = 0.0f;
    int m_maxIterations = 500;

    float m_realShift = 1.0f; 
    float m_imagShift = 0.0f;   
    float m_distortion = 0.25f;  

    void UploadExtraUniforms() override;

public:
    GPUCollatz(std::string name);
    virtual ~GPUCollatz() = default;

    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};