#pragma once
#include <string>
#include "../external/imgui/imgui.h"
#include "ControlComponent.hpp"

class AmountSliderComponent : public ControlComponent {
private:
    float m_amount = 1.0f;
    std::string m_label;
    std::string m_imguiID;

public:
    AmountSliderComponent(std::string label) 
        : m_label(std::move(label)), m_imguiID("##" + label) {}

    void DrawControlPanel() override {
        ImGui::PushID(this);
        ImGui::Text("%s Amount", m_label.c_str());
        ImGui::SliderFloat(m_imguiID.c_str(), &m_amount, 0.0f, 1.0f);
        ImGui::PopID();
    }

    float GetAmount() const { return m_amount; }
};