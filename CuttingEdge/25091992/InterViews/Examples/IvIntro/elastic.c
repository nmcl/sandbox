#include <InterViews/world.h>
#include <InterViews/fixedspan.h>
#include <InterViews/rule.h>

#include "elastic.h"

Elastic::Elastic() {}

HElastic::HElastic() {}

VElastic::VElastic() {}

void HElastic::resize(Coord w, Coord)
{
    body(new FixedSpan(new HRule(World::current()->foreground(), 1.0),
                                 Dimension_X, w));
}

void VElastic::resize(Coord, Coord h)
{
    body(new FixedSpan(new VRule(World::current()->foreground(), 1.0),
                                 Dimension_Y, h));
}
