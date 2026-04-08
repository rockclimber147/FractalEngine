#pragma once
#include <GL/glew.h> // Ensure GLEW is before GLFW
#include <GLFW/glfw3.h>
#include "FractalComponent.hpp"

class GPUFractalComponent : public FractalComponent {
protected:
    GLuint m_fbo = 0;
    GLuint m_shaderProgram = 0;
    GLuint m_vao = 0, m_vbo = 0;

    // GPU-specific setup
    void InitGLResources();
    
    // The "Hook": Subclasses (like MandelbrotGPU) provide the GLSL code
    virtual const char* GetFragmentShaderSource() = 0;
    virtual void UploadExtraUniforms() {} // For things like max_iterations

public:
    GPUFractalComponent();
    virtual ~GPUFractalComponent();

    // Implements the Shader-dispatch logic
    void UpdateTexture() override;
    void Resize(int w, int h) override;
};