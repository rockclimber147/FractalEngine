#include "fractals/core/CPUFractalComponent.hpp"

class CPUBarnsleyFern : public CPUFractalComponent {
protected:
    float m_mainScale = 0.85f;
    float m_mainCurvature = 0.04f;
    float m_mainHeight = 1.6f;

    float m_sideScale = 0.23f;
    float m_sideRotation = 0.24f;
    
    int m_iterationCount = 2000000;

public:
    CPUBarnsleyFern(std::string name);
    virtual ~CPUBarnsleyFern() = default;

    void GenerateFractalData() override;
    void DrawControlPanel() override;
};