/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * PortReference.java
 */

package com.arjuna.wsc.messaging;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;

public class PortReference
{
    public PortReference()
    {
    }

    public PortReference(String address)
    {
        setAddress(address);
    }

    public void setAddress(String address)
    {
        _address = address;
    }

    public String getAddress()
    {
        return _address;
    }

    public void addExtension(String tag, String value)
    {
        _extensions.add(new Extension(tag, value));
    }

    public String getExtensionValue(String tag)
    {
        String   extensionValue = null;
        Iterator iterator       = _extensions.iterator();
        while (iterator.hasNext() && (extensionValue == null))
        {
            Extension extension = (Extension) iterator.next();

            if (tag.equals(extension.getTag()))
                extensionValue = extension.getValue();
        }

        return extensionValue;
    }

    public Iterator getExtensions()
    {
        return _extensions.iterator();
    }

    public void clearExtensions()
    {
        _extensions.clear();
    }

    public static class Extension
    {
        public Extension(String tag, String value)
        {
            _tag   = tag;
            _value = value;
        }

        public String getTag()
        {
            return _tag;
        }

        public String getValue()
        {
            return _value;
        }

        private String _tag   = null;
        private String _value = null;
    }

    private String _address    = null;
    private List   _extensions = new LinkedList();
}
