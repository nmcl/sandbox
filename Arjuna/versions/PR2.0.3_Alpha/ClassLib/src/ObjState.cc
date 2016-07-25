/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjState.cc,v 1.14 1993/10/08 13:19:30 ngdp Exp $
 */

/*
 * Object State Class
 *
 * This class is used to hold both the recovery data and
 * persistent data required by a class derived from
 * StateManager.
 *
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

static const char RCSid[] = "$Id: ObjState.cc,v 1.14 1993/10/08 13:19:30 ngdp Exp $";

/*
 * Public constructors and destructor
 */

/*
 * Default constructor for an empty ObjectState.
 */

ObjectState::ObjectState ()
                         : Buffer(),
			   bufferUid(),
                           imageType(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::ObjectState()\n" << flush;
#endif
}

/*
 * Constructor that creates an ObjectState that is a copy of another
 */

ObjectState::ObjectState ( const ObjectState& copyFrom ) 
                         : Buffer(copyFrom),
			   bufferUid(copyFrom.stateUid()),
			   imageType(0)
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::ObjectState(" << (void *)&copyFrom << ")\n" << flush;
#endif


    /*
     * Fix up the pointers so the copyFrom can be deleted
     * independently - uid already taken care of by initialisation
     */

    const TypeName temp = copyFrom.type(); 

    copytype(temp);
}

/*
 * ObjectState with known Uid
 */

ObjectState::ObjectState ( const Uid& newUid, TypeName tName )
                         : Buffer(),
			   bufferUid(newUid),
			   imageType(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::ObjectState(" << newUid
                 << ", " << tName << ")\n" << flush;
#endif

    copytype(tName);
}


/*
 * ObjectState constructor taking a char* buffer which is to be managed by the
 * Buffer base class. Hence, the char* buffer MUST NOT be deleted.
 */

ObjectState::ObjectState ( const Uid& newUid, TypeName tName, size_t size,
			   char *buffer )
                         : Buffer(size, buffer),
			   bufferUid(newUid),
			   imageType(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::ObjectState(" << newUid
	         << ", " << tName << ", " << size << ", " << (void *)buffer << ")\n" << flush;
#endif

    copytype(tName);
}

ObjectState::~ObjectState ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::~ObjectState() for " << bufferUid << "\n" << flush;
#endif

    if (imageType)
#ifndef __GNUG__
	delete [] imageType;
#else
        delete imageType;
#endif
}

/*
 * Public non-virtual functions
 */

char *ObjectState::buffer () const
{
    return (Buffer::buffer());
}

Boolean ObjectState::notempty () const
{
    return ((length() > 0) ? TRUE : FALSE);
}

size_t ObjectState::size () const
{
    return (length());
}

const Uid& ObjectState::stateUid() const
{
    return bufferUid;
}

const TypeName ObjectState::type () const
{
    return imageType;
}

ostream& ObjectState::print ( ostream& strm ) const
{
    strm << "ObjectState Uid   : " << bufferUid << "\n";

    if (imageType != 0)
	strm << "ObjectState Type  : " << imageType << "\n";

    strm << "ObjectState Size  : " << size() << "\n";
    strm << "ObjectState Buffer: ";
    Buffer::print(strm);

    return strm;
}

ObjectState& ObjectState::operator= ( const ObjectState& objstate )
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "ObjectState::operator= for " << bufferUid << "\n" << flush;
#endif

    Buffer::operator= (objstate);

    bufferUid = objstate.bufferUid;
    imageType = objstate.imageType;

    return *this;
}

Boolean ObjectState::packInto ( Buffer& buff ) const
{
    if (buff.pack(imageType) &&
	bufferUid.pack(buff) &&
	Buffer::packInto(buff))
	return TRUE;
    else
	return FALSE;
}

Boolean ObjectState::unpackFrom ( Buffer& buff )
{
    if (buff.unpack(imageType) &&
	bufferUid.unpack(buff) &&
	Buffer::unpackFrom(buff))
	return TRUE;
    else
	return FALSE;
}

void ObjectState::marshall ( RpcBuffer& buff ) const
{
    if (buff.pack(imageType) && bufferUid.pack(buff))
	Buffer::marshall(buff);
}

void ObjectState::unmarshall ( RpcBuffer& buff)
{
    if (buff.unpack(imageType) && bufferUid.unpack(buff))
	Buffer::unmarshall(buff);
}

/*
 * Private non-virtual functions
 */

void ObjectState::copytype ( const TypeName tName )
{
    if (tName && (strlen(tName) > 0))
    {
	imageType = new char[strlen((char *) tName) + 1];

	strcpy((char *) imageType, (char *) tName);
    }
    else
	imageType = 0;
}


#ifdef NO_INLINES
#  define OBJSTATE_CC_
#  include "Arjuna/ObjState.n"
#  undef OBJSTATE_CC_
#endif
