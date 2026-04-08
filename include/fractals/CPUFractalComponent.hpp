#pragma once
#include "FractalComponent.hpp"
#include <vector>

class CPUFractalComponent : public FractalComponent {
protected:
    std::vector<unsigned char> m_pixelBuffer;

    // CPU-specific GL helpers
    void InitTexture();
    void UploadTexture();

public:
    CPUFractalComponent();
    virtual ~CPUFractalComponent() = default;

    // Implements the double-for-loop logic
    void UpdateTexture() override;
    void Resize(int w, int h) override;

    // The "Hook": Subclasses (like MandelbrotCPU) implement this
    virtual void GetPixelColor(double u, double v, unsigned char rgb[3]) = 0;
};