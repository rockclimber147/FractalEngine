#pragma once

class ControlComponent {
public:
    virtual ~ControlComponent() {}
    virtual void DrawControlPanel() = 0;
    virtual std::string GetLabel() const = 0;
};