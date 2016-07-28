/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverAR.h,v 1.4 1993/12/14 11:28:48 nmcl Exp $
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
    friend class CadaverASetUp;

public:
    /* Constructors and destructor */
    
    CadaverActivationRecord (StateManager *);
    ~CadaverActivationRecord ();
    
    /* inherited functions */

    virtual Boolean propagateOnAbort () const;
    
    virtual Boolean nestedAbort ();
    virtual Boolean nestedCommit ();
    virtual PrepareOutcome nestedPrepare ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    virtual const TypeName type () const;

protected:
    CadaverActivationRecord ();

private:
    virtual Boolean shouldReplace (const AbstractRecord *) const;
};


class CadaverASetUp
{
public:
    CadaverASetUp ();
    ~CadaverASetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static CadaverASetUp CadASetUp;


#endif





























