#include "adaptable.h"
#include "elaboration.h"

Adaptable::Adaptable(Coord w, Coord h) 
         : Resizable(new Elaboration(w, h)) {}

void Adaptable::resize(Coord w, Coord h)
{
    ((Elaboration *) body())->resize(w, h);
}
