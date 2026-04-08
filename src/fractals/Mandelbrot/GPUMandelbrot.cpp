#include "fractals/Mandelbrot/GPUMandelbrot.hpp"

GPUMandelbrot::GPUMandelbrot(std::string name) 
    : m_name(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

std::string GPUMandelbrot::GetLabel() const { return m_name; }

const char* GPUMandelbrot::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec2 u_offset;
        uniform float u_zoom;
        uniform vec2 u_resolution;

        void main() {
            float ppu = 200.0 * u_zoom;
            float u = u_offset.x + (gl_FragCoord.x - u_resolution.x * 0.5) / ppu;
            float v = u_offset.y + (gl_FragCoord.y - u_resolution.y * 0.5) / ppu;

            vec2 c = vec2(u, v);
            vec2 z = vec2(0.0);
            int iter = 0;
            while(dot(z, z) <= 4.0 && iter < 500) {
                z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
                iter++;
            }
            FragColor = (iter == 500) ? vec4(0,0,0,1) : vec4(vec3(float(iter)/500.0), 1.0);
        }
    )";
}

void GPUMandelbrot::UploadExtraUniforms() {
    // TODO
}

void GPUMandelbrot::DrawControlPanel() {
    ImGui::Text("Mandelbrot");
    
    // Increased max iterations for better detailed edges
    if (ImGui::SliderInt("Detail (Iterations)", &m_maxIterations, 50, 2000)) {
        UpdateTexture();
    }

    // New Slider: Controls color density/cycling speed
    if (ImGui::SliderFloat("Color Cycle Speed", &m_colorFrequency, 1.0f, 100.0f)) {
        UpdateTexture();
    }

    if (ImGui::SliderFloat("Color Offset", &m_colorOffset, 0.0f, 6.0f)) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

