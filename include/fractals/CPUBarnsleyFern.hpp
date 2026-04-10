#include "fractals/core/CPUFractalComponent.hpp"

class CPUBarnsleyFern : public CPUFractalComponent {

public:
    CPUBarnsleyFern(std::string name);
    virtual ~CPUBarnsleyFern() = default;

    void GenerateFractalData() override;
    void DrawControlPanel() override;
};