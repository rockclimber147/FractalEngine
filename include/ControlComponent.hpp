#pragma once

class ControlComponent {
public:
    virtual ~ControlComponent() {}
    virtual void DrawControlPanel() = 0;
};