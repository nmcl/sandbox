/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Uid.cc,v 1.5 1997/05/16 09:08:27 nmcl Exp $
 */

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef OS_STDIO_H_
#  include <os/stdio.h>
#endif

#ifndef OS_TIME_H_
#  include <os/time.h>
#endif

#ifdef WIN32

#   ifndef OS_WINSOCK_H_
#       include <os/winsock.h>
#   endif

#   ifndef OS_PROCESS_H_
#       include <os/process.h>
#   endif

#else

#   ifndef OS_SYS_PARAM_H_
#       include <os/sys/param.h>
#   endif

#   if defined(HAS_SYSINFO) && !defined(OS_SYS_SYSTEMINFO_H_)
#       include <os/sys/systeminfo.h>
#   endif

#   ifndef OS_SYS_SOCKET_H_
#       include <os/sys/socket.h>
#   endif

#   ifndef OS_UNISTD_H_
#       include <os/unistd.h>
#   endif

#   ifndef OS_NETINET_IN_H_
#       include <os/netinet/in.h>
#   endif

#   ifndef OS_NETDB_H_
#       include <os/netdb.h>
#   endif

#   ifndef OS_ARPA_INET_H_
#       include <os/arpa/inet.h>
#   endif

#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#   include <Interface/Buffer.h>
#endif

Uid* Uid::_invalid = NULL;

unsigned long int Uid::_cacheValue0 = 0;
unsigned long int Uid::_cacheValue1 = 0;
unsigned long int Uid::_cacheValue2 = 0;
unsigned long int Uid::_lastValue3  = 0;


Uid::Uid()
{
    if (_cacheValue0 == 0)
    {
        struct hostent *hostEnt;
        char hostName[MAXHOSTNAMELEN];

#ifdef HAS_SYSINFO
        if (sysinfo(SI_HOSTNAME, hostName, MAXHOSTNAMELEN) > 0)
#else
        if (gethostname(hostName, MAXHOSTNAMELEN) == 0)
#endif
            if ((hostEnt = gethostbyname(hostName)) != NULL)
                memcpy(&_cacheValue0, hostEnt->h_addr_list[0],
		       hostEnt->h_length);
    }
    
    _value0 = _cacheValue0;
    
    if (_cacheValue1 == 0)
#ifdef WIN32
        _cacheValue1 = (unsigned long int) _getpid();
#else
        _cacheValue1 = (unsigned long int) getpid();
#endif
    
    _value1 = _cacheValue1;
    
    if (_cacheValue2 == 0)
        _cacheValue2 = (unsigned long int) time(NULL);
    
    _value2 = _cacheValue2;
    _value3 = _lastValue3++;
}

Uid::Uid(const Uid &uid)
{
    _value0 = uid._value0;
    _value1 = uid._value1;
    _value2 = uid._value2;
    _value3 = uid._value3;
}

Uid::Uid(const char *uidString)
{
    if ((uidString == NULL) || (sscanf(uidString, "%lu:%lu:%lu:%lu", &_value3, &_value2, &_value1, &_value0) != 4))
    {
        _value0 = 0;
        _value1 = 0;
        _value2 = 0;
        _value3 = 0;
    }
}

Uid::~Uid()
{
}

unsigned long Uid::hash () const
{
    return 0;
}

Uid& Uid::operator= (const Uid &uid)
{
    if (this == &uid)
	return *this;
    
    _value0 = uid._value0;
    _value1 = uid._value1;
    _value2 = uid._value2;
    _value3 = uid._value3;

    return *this;
}

Boolean Uid::operator== (const Uid &uid) const
{
    return ((_value3 == uid._value3) && (_value2 == uid._value2) &&
            (_value1 == uid._value1) && (_value0 == uid._value0));
}

Boolean Uid::operator!= (const Uid &uid) const
{
    return ((_value3 != uid._value3) || (_value2 != uid._value2) ||
            (_value1 != uid._value1) || (_value0 != uid._value0));
}

Boolean Uid::operator< (const Uid &uid) const
{
    return ((_value3 < uid._value3) || 
            ((_value3 == uid._value3) && ((_value2 < uid._value2) ||
             ((_value2 == uid._value3) && ((_value2 < uid._value1) ||
              ((_value1 == uid._value3) && (_value2 < uid._value0)))))));
}

Boolean Uid::operator> (const Uid &uid) const
{
    return ((_value3 > uid._value3) || 
            ((_value3 == uid._value3) && ((_value2 > uid._value2) ||
             ((_value2 == uid._value3) && ((_value2 > uid._value1) ||
              ((_value1 == uid._value3) && (_value2 > uid._value0)))))));
}

char *Uid::stringForm() const
{
    char *res;
    char str[64];

    ::memset(str, '\0', 64);  // please purify
    sprintf(str, "%lu:%lu:%lu:%lu", _value3, _value2, _value1, _value0);

    res = new char[strlen(str) + 1];
    strcpy(res, str);

    return res;
}

const Uid &Uid::invalid()
{
    if (!_invalid)
	_invalid = new Uid("0:0:0:0");
    
    return *_invalid;
}

Boolean Uid::pack(Buffer &buff) const
{
    return buff.pack(_value0) && buff.pack(_value1) &&
	   buff.pack(_value2) && buff.pack(_value3);
}

Boolean Uid::unpack(Buffer &buff)
{
    return buff.unpack(_value0) && buff.unpack(_value1) &&
	   buff.unpack(_value2) && buff.unpack(_value3);
}

void Uid::print(ostream &ostr) const
{
    ostr << "<Uid:" << _value3 << ':' << _value2 << ':'
	 << _value1 << ':' << _value0 << '>';
}

ostream &operator<< (ostream &ostr, const Uid &uid)
{
    uid.print(ostr);
    return ostr;
}
