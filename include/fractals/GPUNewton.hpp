#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPUNewton : public GPUFractalComponent {
private:
    float m_a = 1.0f, m_b = 0.0f, m_c = 0.0f, m_d = -1.0f;
    int m_maxIterations = 100;
    
protected:
    void UploadExtraUniforms() override;

public:
    GPUNewton(std::string name);
    virtual ~GPUNewton() = default;

    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};