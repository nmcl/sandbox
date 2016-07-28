#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef WIN32
#  ifndef OS_NETINET_IN_H_
#    include <os/netinet/in.h>
#  endif
#else
#  include <os/winsock.h>
#endif

#ifndef OS_LIMITS_H_
#  include <os/limits.h>
#endif

#ifndef OS_CTYPE_H_
#  include <os/ctype.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef IMPLEMENTATION_ARJUNA_OBJBUFFER_H_
#  include <Implementation/Arjuna_ObjBuffer.h>
#endif

#ifndef COMMON_OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

/* 
 * Public constructors and destructors
 */

Arjuna_ObjectState::Arjuna_ObjectState ()
				       : _theBuffer(new ObjectState)
{
}

Arjuna_ObjectState::Arjuna_ObjectState (ObjectState* buff)
				       : _theBuffer(buff)
{
}
     
Arjuna_ObjectState::~Arjuna_ObjectState ()
{
    if (_theBuffer)
	delete _theBuffer;
}

ObjectState* Arjuna_ObjectState::buffer () const
{
    return _theBuffer;
}

void Arjuna_ObjectState::buffer (ObjectState* buff)
{
    if (!buff)
	return;
    
    if (_theBuffer)
	delete _theBuffer;

    _theBuffer = buff;
}

/* 
 * Public non-virtual member functions and operators
 */

char* Arjuna_ObjectState::contents (CORBA_(Environment& env))
{
    return (char*) _theBuffer->buffer();
}

long Arjuna_ObjectState::length (CORBA_(Environment& env))
{
    return _theBuffer->size();
}

ObjectBuffer::Status Arjuna_ObjectState::readState (CORBA_(Environment& env))
{
    return (ObjectBuffer::Status) _theBuffer->readState();
}

void Arjuna_ObjectState::packULong ( unsigned long l, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(l))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packFloat ( float f, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(f))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packDouble ( double d, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(d))
	throw ObjectBuffer::PackError();    
}

/* a null terminated string */

void Arjuna_ObjectState::packMappedString ( const char *string, CORBA_(Environment& env) )
{
    if (!_theBuffer->packMappedString(string))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packNonMappedString ( const char *string, CORBA_(Environment& env) )
{
    if (!_theBuffer->packNonMappedString(string))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packChar ( char c, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(c))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packBoolean ( CORBA::Boolean b, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(b))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packShort ( short s, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(s))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packUShort ( unsigned short s, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(s))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::packLong ( long l, CORBA_(Environment& env) )
{
    if (!_theBuffer->pack(l))
	throw ObjectBuffer::PackError();
}

void Arjuna_ObjectState::unpackChar ( char& c, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(c))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackShort ( short& s, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(s))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackBoolean ( CORBA::Boolean& b, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(b))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackUShort ( unsigned short& s, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(s))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackLong ( long& l, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(l))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackULong ( unsigned long& l, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(l))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackFloat ( float& f, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(f))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackDouble ( double& d, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(d))
	throw ObjectBuffer::UnpackError();
}

void Arjuna_ObjectState::unpackString ( char *& string, CORBA_(Environment& env) )
{
    if (!_theBuffer->unpack(string))
	throw ObjectBuffer::UnpackError();
}
