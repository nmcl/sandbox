#include "Placement.h"

Placement::Placement () {}

Placement::~Placement () {}

void Placement::RandomPlacementPolicy (Machine* Sites[], Object* obj)
{
    RP = new RandomPolicy(Sites, obj);
}

/* Parameters: Machines, Objects to be placed, require Availability for replica group,
 * and the expected read/write ratio for the replica group.
 */

void Placement::SimplePlacementPolicy (Machine* Sites[], Object* obj, double Availability, double opratio = 0.9)
{
    SP = new SimplePolicy(Sites, obj, Availability, opratio);
}
