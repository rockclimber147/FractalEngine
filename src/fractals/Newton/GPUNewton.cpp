#include "fractals/Newton/GPUNewton.hpp"

GPUNewton::GPUNewton(std::string name) 
    : m_name(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

std::string GPUNewton::GetLabel() const { return m_name; }

const char* GPUNewton::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec2 u_resolution;
        uniform float u_zoom;
        uniform vec2 u_offset;

        vec2 complexDiv(vec2 a, vec2 b) {
            float den = dot(b, b);
            return vec2(dot(a, b), a.y * b.x - a.x * b.y) / den;
        }

        vec2 complexMul(vec2 a, vec2 b) {
            return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
        }

        void main() {
            float ppu = 200.0 * u_zoom;
            vec2 z = u_offset + (gl_FragCoord.xy - u_resolution * 0.5) / ppu;

            int root = -1;
            int iter = 0;
            
            for(iter = 0; iter < 100; iter++) {
                // For f(z) = z^3 - 1, f'(z) = 3z^2
                vec2 z2 = complexMul(z, z);
                vec2 fz = complexMul(z2, z) - vec2(1.0, 0.0);
                vec2 dfz = 3.0 * z2;
                
                // Newton step: z = z - f(z)/f'(z)
                z -= complexDiv(fz, dfz);

                // Check proximity to the 3 roots of unity
                float eps = 0.001;
                if (length(z - vec2(1.0, 0.0)) < eps) { root = 0; break; }
                if (length(z - vec2(-0.5, 0.866)) < eps) { root = 1; break; }
                if (length(z - vec2(-0.5, -0.866)) < eps) { root = 2; break; }
            }

            // Coloring logic
            vec3 baseCol;
            if (root == 0) baseCol = vec3(1.0, 0.3, 0.3); // Reddish
            else if (root == 1) baseCol = vec3(0.3, 1.0, 0.3); // Greenish
            else if (root == 2) baseCol = vec3(0.3, 0.3, 1.0); // Bluish
            else baseCol = vec3(0.0); // Did not converge

            // Mix in the iteration count for shading/depth
            float brightness = 1.0 - float(iter) / 100.0;
            FragColor = vec4(baseCol * brightness, 1.0);
        }
    )";
}

void GPUNewton::UploadExtraUniforms() {
}

void GPUNewton::DrawControlPanel() {
    bool changed = false;
}

