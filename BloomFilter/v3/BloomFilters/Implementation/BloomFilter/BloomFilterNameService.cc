/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilterNameService.cc,v 1.1 1998/06/02 16:51:16 nmcl Exp $
 */

#include <System/iostream.h>
#include <System/string.h>

#ifndef BLOOMFILTERNAMESERVICE_H_
#  include "BloomFilterNameService.h"
#endif

class FilterContext
{
public:
    FilterContext ();
    ~FilterContext ();

    BloomFilter* _theFilter;
    char* _ior;
};

FilterContext::FilterContext ()
			     : _theFilter(0),
			       _ior(0)
{
}

FilterContext::~FilterContext ()
{
    if (_theFilter)
	delete _theFilter;

    if (_ior)
	::delete [] _ior;
}


BloomFilterNameService::BloomFilterNameService (size_t size)
					       : _filters(0),
						 _size(size),
						 _index(0)
{
    if (_size > 0)
    {
	_filters = new FilterContext* [_size];

	for (size_t i = 0; i < _size; i++)
	    _filters[i] = NULL;
    }
}

BloomFilterNameService::~BloomFilterNameService ()
{
    if (_size > 0)
    {
	for (size_t i = 0; i < _size; i++)
	    if (_filters[i])
		delete _filters[i];

	delete [] _filters;
    }
}

Boolean BloomFilterNameService::insert (BloomFilter* filter, const char* ior)
{
    if (_index < _size)
    {
	_filters[_index] = new FilterContext;

	_filters[_index]->_theFilter = filter;
	_filters[_index]->_ior = ::new char[::strlen(ior)+1];
	::strcpy(_filters[_index]->_ior, ior);

	_index++;

	return TRUE;
    }
    else
	return FALSE;
}

Boolean BloomFilterNameService::present (const Uid& objId) const
{
    for (int i = 0; i < _size; i++)
    {
	if (_filters[i] != NULL)
	{
	    if (_filters[i]->_theFilter->present(objId))
		return TRUE;
	}
    }

    return FALSE;
}

char** BloomFilterNameService::locate (const Uid& objId, int& size) const
{
    char** toReturn = NULL;
    size_t i = 0;
    
    size = 0;

    for (i = 0; i < _size; i++)
    {
	if (_filters[i] != NULL)
	{
	    if (_filters[i]->_theFilter->present(objId))
		size++;
	}
    }

    if (size > 0)
    {
	size_t index = 0;
	
	toReturn = ::new char* [size];
	
	for (i = 0; i < _size; i++)
	{
	    if (_filters[i] != NULL)
	    {
		if (_filters[i]->_theFilter->present(objId))
		{
		    toReturn[index] = ::new char[::strlen(_filters[i]->_ior)+1];
		    ::strcpy(toReturn[index], _filters[i]->_ior);
		    
		    index++;
		}
	    }
	}
    }

    return toReturn;
}
