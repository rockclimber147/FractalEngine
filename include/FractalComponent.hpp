#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include "Component.hpp"

class FractalComponent : public Component {
protected:
    // Viewport State: Camera center and zoom level
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
    double m_zoom = 1.0;

    // Rendering State
    int m_width = 1;
    int m_height = 1;
    GLuint m_textureID = 0;
    std::vector<unsigned char> m_pixelBuffer;

    // Internal GL Helpers
    void InitTexture();
    void UploadTexture();

public:
    FractalComponent();
    virtual ~FractalComponent();

    // Shared Viewport Implementations
    void Pan(float dx, float dy, float vW, float vH) override;
    void Zoom(float amount, float mouseX, float mouseY, float vW, float vH) override;
    void Resize(int w, int h) override;

    // ImGui Interface Implementations
    ImTextureID GetResultTexture() const override;
    
    // The "Hook": Subclasses must implement the actual pixel loop
    virtual void UpdateTexture() = 0;
};