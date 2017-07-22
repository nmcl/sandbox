/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef SET_H_
#include "Set.h"
#endif

#ifndef ELEMENT_H_
#include "Element.h"
#endif

#include <iostream.h>


int main ()
{
    Set S1, S2;
    Set *S3 = 0;
    Element* E = 0;
    
    for (int i = 0; i < 10; i++)
    {
	E = new Element(i);
	E->InTo(&S1);
    }
    
    for (int j = 8; j < 14; j++)
    {
	E = new Element(j);
	E->InTo(&S2);
    }

    S3 = S1.Intersect(&S2);
    
    E = (Element*) S3->First();
    
    cout << "Intersection is:" << endl;
    while (E)
    {
	cout << "value: " << E->GetValue() << endl;
	E = (Element*) E->Suc();
    }
    
    return 0;
}
