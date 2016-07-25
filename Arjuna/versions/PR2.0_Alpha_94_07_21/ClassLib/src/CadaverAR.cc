/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverAR.cc,v 1.5 1993/12/14 11:31:02 nmcl Exp $
 */

/*
 *
 * CadaverActivationRecord Class
 *
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef CADAVERAR_H_
#  include "CadaverAR.h"
#endif

static const char RCSid[] = "$Id: CadaverAR.cc,v 1.5 1993/12/14 11:31:02 nmcl Exp $";


/*
 * Public constructors and destructors
 */

Boolean CadaverASetUp::setUp = FALSE;


CadaverASetUp::CadaverASetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverASetUp::CadaverASetUp ()" << endl;
#endif
    
    if (!setUp)
    {
        CadaverActivationRecord tr;
	AbstractRecord::addToList(CadaverASetUp::create, CadaverASetUp::remove, tr.type());
	setUp = TRUE;
    }
}

CadaverASetUp::~CadaverASetUp () {}

AbstractRecord* CadaverASetUp::create ()
{
    return new CadaverActivationRecord;
}

void CadaverASetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * This constructor is used to create a new instance of an
 * CadaverActivationRecord.
 */

CadaverActivationRecord::CadaverActivationRecord ( StateManager *sm )
                                                 : ActivationRecord(PASSIVE, sm)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::CadaverActivationRecord("
		 << sm ->get_uid() << ")" << endl;
#endif
}

CadaverActivationRecord::CadaverActivationRecord ()
                                                  : ActivationRecord()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "CadaverActivationRecord::CadaverActivationRecord ()" << endl;
#endif
}

CadaverActivationRecord::~CadaverActivationRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::~CadaverActivationRecord() for " 
		 << order() << endl;
#endif

}

/*
 * Public virtual functions and operators
 */

Boolean CadaverActivationRecord::propagateOnAbort () const
{
    return TRUE;
}

/*
 * Supress all atomic action ops for deleted object
 */

Boolean CadaverActivationRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::nestedAbort() for " 
		 << order() << endl;
#endif
    
    return TRUE;
}

Boolean CadaverActivationRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::nestedCommit() for " 
		 << order() << endl;
#endif
    
    return TRUE;
}

PrepareOutcome CadaverActivationRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::nestedPrepare() for " 
		 << order() << endl;
#endif

    return READONLY;
}

Boolean CadaverActivationRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::topLevelAbort() for " 
		 << order() << endl;
#endif
    
    return TRUE;
}

Boolean CadaverActivationRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::topLevelCommit() for " 
		 << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome CadaverActivationRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::topLevelPrepare() for " << order() << endl;
#endif

    return READONLY;
}

const TypeName CadaverActivationRecord::type () const
{
    return ("/StateManager/AbstractRecord/CadaverActivationRecord");
}

/*
 * shouldMerge and should_replace are invoked by the record list manager
 * to determine if two records should be merged togethor or if the
 * 'newer' should replace the older.
 * shouldAdd determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * shouldMerge and shouldReplace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean CadaverActivationRecord::shouldReplace ( const AbstractRecord * ar ) const
{
    return  (((order() == ar->order()) &&
	     ar->typeIs() == ACTIVATION ) ? TRUE : FALSE);
}
