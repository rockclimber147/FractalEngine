#pragma once

#include "fractals/GPUFractalComponent.hpp"
#include <string>

class GPUNewton : public GPUFractalComponent {
private:
    std::string m_name;
    int m_sequence[16] = {0, 0, 1, 0, 1}; // AABAB (0=A, 1=B)
    int m_seqLength = 5;
    int m_maxIterations = 200;

protected:
    const char* GetFragmentShaderSource() override;
    void UploadExtraUniforms() override;

public:
    GPUNewton(std::string name);
    virtual ~GPUNewton() = default;

    std::string GetLabel() const override;
    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};