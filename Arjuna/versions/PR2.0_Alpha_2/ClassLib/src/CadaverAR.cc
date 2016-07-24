/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverAR.cc,v 1.1 1993/11/03 12:29:45 nmcl Exp $
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

static const char RCSid[] = "$Id: CadaverAR.cc,v 1.1 1993/11/03 12:29:45 nmcl Exp $";

/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * CadaverActivationRecord.
 */

CadaverActivationRecord::CadaverActivationRecord ( StateManager *sm )
                                                 : ActivationRecord(PASSIVE, sm)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::CadaverActivationRecord()\n" << flush;
#endif
}

CadaverActivationRecord::~CadaverActivationRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::~CadaverActivationRecord()\n" << flush;
#endif

}

/*
 * Public virtual functions and operators
 */

/*
 * should_merge and should_replace are invoked by the record list manager
 * to determine if two records should be merged togethor or if the
 * 'newer' should replace the older.
 * should_add determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean CadaverActivationRecord::should_replace ( const AbstractRecord * ar ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::should_replace(" << (void *)ar << ")\n" << flush;
#endif

    return  (((ordering() == ar->ordering()) &&
	     ar->type_is() == ACTIVATION ) ? TRUE : FALSE);
}

/*
 * nested_abort causes the reset_state function of the object to be invoked
 * passing it the saved ObjectStatus
 */

void CadaverActivationRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::nested_abort()\n" << flush;
#endif

}/*
 * top_level_commit has nothing to do for CadaverActivationRecords
 */

void CadaverActivationRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverActivationRecord::top_level_commit()\n" << flush;
#endif

}

const TypeName CadaverActivationRecord::type () const
{
    return ("/StateManager/AbstractRecord/CadaverActivationRecord");
}
