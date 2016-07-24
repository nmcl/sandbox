/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Mailer.h,v 1.1 1994/08/16 12:31:43 ndbi Exp $
 */

#ifndef MAILER_H_
#define MAILER_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class Mailer : public ofstream
{
    
public:
    Mailer(ostream&);
    ~Mailer();

    void sendMessage();
    
private:
    ostream& outStrm;
    char *fileName;
    char *mailProg;
    char *recipient;
    char *subject;
};

#endif /* MAILER_H_ */
