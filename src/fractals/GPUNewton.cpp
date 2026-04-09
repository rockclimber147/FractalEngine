#include "fractals/GPUNewton.hpp"

GPUNewton::GPUNewton(std::string name) 
    : GPUFractalComponent(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

void GPUNewton::UploadExtraUniforms() {
}

void GPUNewton::DrawControlPanel() {
    bool changed = false;
}

