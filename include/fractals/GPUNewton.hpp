#pragma once

#include "fractals/core/GPUFractalComponent.hpp"
#include <string>

class GPUNewton : public GPUFractalComponent {
private:
    std::string m_name;

protected:
    void UploadExtraUniforms() override;

public:
    GPUNewton(std::string name);
    virtual ~GPUNewton() = default;

    void DrawControlPanel() override;
    
    double GetPixelsPerUnit() const override { return 200.0 * m_zoom; }
};