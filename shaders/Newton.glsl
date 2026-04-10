#version 330 core
#extension GL_ARB_gpu_shader_fp64 : enable 

out vec4 FragColor;
uniform dvec2 u_offset;
uniform double u_zoom;
uniform vec2 u_resolution; 
uniform int u_maxIterations;

// Polynomial Coefficients: az^3 + bz^2 + cz + d
uniform float u_a, u_b, u_c, u_d;

dvec2 complexMul(dvec2 a, dvec2 b) {
    return dvec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

dvec2 complexDiv(dvec2 a, dvec2 b) {
    double den = dot(b, b);
    return dvec2(dot(a, b), a.y * b.x - a.x * b.y) / den;
}

void main() {
    double ppu = 200.0lf * u_zoom;
    dvec2 flippedCoord = dvec2(gl_FragCoord.x, double(u_resolution.y) - gl_FragCoord.y);
    dvec2 z = u_offset + (flippedCoord - dvec2(u_resolution * 0.5f)) / ppu;

    int iter = 0;
    dvec2 oldZ;
    
    // Convert float uniforms to doubles for math
    double a = double(u_a);
    double b = double(u_b);
    double c = double(u_c);
    double d = double(u_d);

    for(iter = 0; iter < u_maxIterations; iter++) {
        oldZ = z;
        dvec2 z2 = complexMul(z, z);
        dvec2 z3 = complexMul(z2, z);

        // f(z) = az^3 + bz^2 + cz + d
        dvec2 fz = a * z3 + b * z2 + c * z + dvec2(d, 0.0lf);
        
        // f'(z) = 3az^2 + 2bz + c
        dvec2 dfz = 3.0lf * a * z2 + 2.0lf * b * z + dvec2(c, 0.0lf);
        
        if (dot(dfz, dfz) < 1e-12) break;
        
        z -= complexDiv(fz, dfz);

        // If z has stopped moving significantly, we've found a root
        if (length(z - oldZ) < 1e-7) break;
    }

    // COLORING BY ANGLE
    // Since we don't know where the roots are, we color based on the 
    // angle of the final z-coordinate (the root it found).
    float angle = float(atan(float(z.y), float(z.x)));
    
    // Create a smooth hue based on the angle
    vec3 baseCol = 0.5 + 0.5 * cos(angle + vec3(0, 2.0, 4.0));

    // Darken based on iterations
    float brightness = pow(1.0f - float(iter) / float(u_maxIterations), 0.5f);
    
    // Add a black "border" between basins of attraction
    if (iter == u_maxIterations) baseCol = vec3(0.0);

    FragColor = vec4(baseCol * brightness, 1.0);
}