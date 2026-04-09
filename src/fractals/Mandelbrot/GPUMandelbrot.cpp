#include "fractals/Mandelbrot/GPUMandelbrot.hpp"

GPUMandelbrot::GPUMandelbrot(std::string name) 
    : GPUFractalComponent(std::move(name))
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

void GPUMandelbrot::UploadExtraUniforms() {
    GLint maxIterLoc = glGetUniformLocation(m_shaderProgram, "u_maxIterations");
    GLint colorFreqLoc = glGetUniformLocation(m_shaderProgram, "u_colorFrequency");
    GLint colorOffsetLoc = glGetUniformLocation(m_shaderProgram, "u_colorOffset");

    glUniform1i(maxIterLoc, m_maxIterations);
    glUniform1f(colorFreqLoc, m_colorFrequency);
    glUniform1f(colorOffsetLoc, m_colorOffset);
}

void GPUMandelbrot::DrawControlPanel() {
    bool changed = false;

    if (ImGui::SliderInt("Max Iterations", &m_maxIterations, 1, 2000)) {
        changed = true;
    }

    if (ImGui::SliderFloat("Color Cycle Speed", &m_colorFrequency, 1.0f, 100.0f)) {
        changed = true;
    }

    if (ImGui::SliderFloat("Color Offset", &m_colorOffset, 0.0f, 6.0f)) {
        changed = true;
    }

    if (changed) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

