#include "Placement.h"

Placement::Placement() {}

Placement::~Placement() {}

void Placement::RandomPlacementPolicy(Machine* Sites[], Object* obj)
{
    RP = new RandomPolicy(Sites, obj);
}

void Placement::SimplePlacementPolicy(Machine* Sites[], Object* obj, double Availability, double opratio = 0.9)
{
    SP = new SimplePolicy(Sites, obj, Availability, opratio);
}
