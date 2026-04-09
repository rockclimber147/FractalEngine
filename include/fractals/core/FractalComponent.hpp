#pragma once
#include <GLFW/glfw3.h>
#include "Component.hpp"

class FractalComponent : public Component {
protected:
    std::string m_name;

    // Shared Viewport State
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
    double m_zoom = 1.0;

    // Shared Rendering Metadata
    int m_width = 1;
    int m_height = 1;
    GLuint m_textureID = 0;

public:
    FractalComponent(std::string m_name);
    virtual ~FractalComponent();

    // Shared logic (Implementation is identical for both)
    void Pan(float dx, float dy, float vW, float vH) override;
    void Zoom(float amount, float mouseX, float mouseY, float vW, float vH) override;

    // Pure Virtual: Subclasses MUST define how they resize and update
    virtual void Resize(int w, int h) = 0;
    virtual void UpdateTexture() = 0;

    // Common Interface
    ImTextureID GetResultTexture() const override;
    virtual double GetPixelsPerUnit() const { return 100.0 * m_zoom; }

    std::string GetLabel() const override;
    void DrawViewportInfo();
};