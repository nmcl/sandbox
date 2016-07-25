/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverAR.h,v 1.2 1993/03/22 09:15:59 ngdp Exp $
 */

#ifndef CADAVERAR_H_
#define CADAVERAR_H_

/*
 *
 * Cadaver Activation Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef ACTIVERR_H_
#  include "ActiveR.h"
#endif

class ostream;
class StateManager;

class CadaverActivationRecord : public ActivationRecord
{
public:
    /* Constructors and destructor */

    CadaverActivationRecord (StateManager *sm);
    ~CadaverActivationRecord ();

    /* inherited functions */
    
    virtual Boolean should_replace (const AbstractRecord *ar) const;

    virtual void nested_abort ();
    virtual void top_level_commit ();

    virtual const TypeName type () const;

};

#endif





























