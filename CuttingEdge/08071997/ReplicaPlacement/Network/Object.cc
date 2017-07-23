/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef OBJECT_H_
#include "Object.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern Machine* Sites[MAX_MACHINES];
extern int NumberAllowed;

ObjectData::ObjectData ()
                        : Lock(unlocked),
			  location(0)
{
    for (int i = 0; i < MaxClients; i++)
	User[i] = 0;
}

ObjectData::~ObjectData () {}

boolean ObjectData::IsUsing (Client* who)
{
    boolean found = false;
    
    for (int i = 0; (i < MaxClients) && (!found); i++)
    {
	if (User[i])
	{
	    if (User[i]->ClientID == who->ClientID)
		found = true;
	}
    }
    
    return found;
}

// Actual Replicated Object class

Object::Object (Protocol* rp, long n_objects)
{
    if (n_objects > MAX_OBJECTS)
    {
	cerr << "Warning::attempt to create " << n_objects << "\n";
	cerr << "Limit is " << MAX_OBJECTS << "\n";
	NumberOfObjects = -1;
	RepProt = 0;
    }
    else
    {
	NumberOfObjects = n_objects;
	RepProt = rp;
    }
}

Object::~Object () {}
	 
Opcode Object::PerformOperation (lock_code l, Client* who) { return RepProt->PerformOperation(l, who); }

boolean Object::CheckQuorum (lock_code l, Client* who) { return RepProt->CheckQuorum(l, who); }

void Object::SetReadQuorum (long RQ) { RepProt->SetReadQuorum(RQ); }
long Object::ReadQuorum () { return RepProt->ReadQuorum; }

void Object::SetWriteQuorum (long WQ) { RepProt->SetWriteQuorum(WQ); }
long Object::WriteQuorum () { return RepProt->WriteQuorum; }

long Object::GetMinimum () { return RepProt->GetMinimum(); }
