#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPULyapunov : public GPUFractalComponent {
private:
    std::string m_name;
    int m_sequence[16] = {0, 0, 1, 0, 1}; // AABAB (0=A, 1=B)
    int m_seqLength = 5;
    int m_maxIterations = 200;

protected:
    void UploadExtraUniforms() override;

public:
    GPULyapunov(std::string name);
    virtual ~GPULyapunov() = default;

    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};