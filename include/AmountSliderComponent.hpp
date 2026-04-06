#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "../external/imgui/imgui.h"
#include "ControlComponent.hpp"

class AmountSliderComponent : public ControlComponent {
private:
    float m_amount = 1.0f;
    std::string m_label;
    std::string m_imguiID;
    GLuint m_textureID = 0;
    unsigned char m_data[3] = { 0, 0, 0 };

public:
    AmountSliderComponent(std::string label) 
        : m_label(std::move(label)), m_imguiID("##" + label) {
            glGenTextures(1, &m_textureID);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            
            // Filtering: Use NEAREST so it stays a sharp block of color
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            UpdateTexture();
        }

    void UpdateTexture() {
        unsigned char val = (unsigned char)(m_amount * 255.0f);
        m_data[0] = (m_label == "Red") ? val : 0;
        m_data[1] = (m_label == "Green") ? val : 0;
        m_data[2] = (m_label == "Blue") ? val : 0;

        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DrawControlPanel() override {
        ImGui::PushID(this);
        ImGui::Text("%s Amount", m_label.c_str());
        if (ImGui::SliderFloat(m_imguiID.c_str(), &m_amount, 0.0f, 1.0f)) {
            UpdateTexture();
        }
        ImGui::PopID();
    }

    float GetAmount() const { return m_amount; }
    std::string GetLabel() const override { return m_label; }

    ImTextureID GetResultTexture() const override {
        return (ImTextureID)(intptr_t)m_textureID;
    }
};