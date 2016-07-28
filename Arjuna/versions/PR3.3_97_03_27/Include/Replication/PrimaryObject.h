/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: PrimaryObject.h,v 1.7 1995/09/12 12:13:46 nmcl Exp $
 */

#ifndef PRIMARYOBEJCT_H_
#define PRIMARYOBJECT_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif


//@NoRemote, @NoMarshall
class PrimaryObject
{
public:
    PrimaryObject (int&);
    ~PrimaryObject ();

    ActionStatus Begin ();
    ActionStatus End   ();
    ActionStatus Abort ();

    Boolean     fetchNameList   (const Buffer&);
    Boolean     fetchNameList   (const ArjunaName&);
    ArjunaName* getPrimary      ();  // do not delete the return value
    Boolean     registerPrimary ();

private:
    void extract ();

    void initialize ();
    void fixBuffers ();

    ArjunaName* findPrimary ();

    ArjunaName     **AN;
    AtomicAction   *Z;
    NSInterface    *NS;
    Buffer         *NameList, *key;
    Boolean        gotPrimary;
    int            index, number, oldPrimary;
};

#endif
