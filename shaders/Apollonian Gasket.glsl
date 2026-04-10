#version 330 core
#extension GL_ARB_gpu_shader_fp64 : enable 

out vec4 FragColor;

uniform dvec2 u_offset;
uniform double u_zoom;
uniform vec2 u_resolution;
uniform float u_colorFrequency;
uniform float u_colorOffset;
uniform float u_foldScale;
uniform float u_spread;

void main() {
    double ppu = 200.0lf * u_zoom;
    
    dvec2 flippedCoord = dvec2(gl_FragCoord.x, double(u_resolution.y) - gl_FragCoord.y);
    
    // Map to complex plane
    dvec2 z = u_offset + (flippedCoord - dvec2(u_resolution * 0.5f)) / ppu;

    double minDist = 1000.0;
    float orbit = 0.0;
    
    // The Gasket logic remains identical
    z *= 0.5; 
    double rLimit = double(u_foldScale * u_foldScale);

    for (int i = 0; i < 30; i++) {
        z.x = abs(z.x);
        z.y = abs(z.y);
        
        dvec2 diff = z - 1.0;
        double r2 = dot(diff, diff);
        
        if (r2 < rLimit) {
            double k = rLimit / r2;
            z = diff * k + 1.0;
        }
        
        z = z * double(u_spread) - (double(u_spread) - 1.0);

        orbit += float(exp(-float(length(z))));
        minDist = min(minDist, length(z));
    }

    float t = orbit * u_colorFrequency;
    vec3 col = 0.5 + 0.5 * cos(3.0 + t + vec3(u_colorOffset, 2.0 + u_colorOffset, 4.0 + u_colorOffset));
    
    float edge = float(smoothstep(0.0, 0.1, minDist));
    col *= edge;

    FragColor = vec4(col, 1.0);
}