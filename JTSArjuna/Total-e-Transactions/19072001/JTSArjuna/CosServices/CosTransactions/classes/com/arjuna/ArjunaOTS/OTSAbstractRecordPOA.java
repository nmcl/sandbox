package com.arjuna.ArjunaOTS;

abstract public class OTSAbstractRecordPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, OTSAbstractRecordOperations, com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourceOperations
{
    public com.arjuna.ArjunaOTS.OTSAbstractRecord _this()
    {
        return com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.narrow(_this_object());
    }

    public com.arjuna.ArjunaOTS.OTSAbstractRecord _this(
        org.omg.CORBA.ORB orb
)
    {
        return com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_OTSAbstractRecord_hashtable.get(opName);
        if (_meth == null)
            throw new org.omg.CORBA.BAD_OPERATION();
        java.lang.Object return_value = null;
        try {
            return_value = _meth.invoke(this, _params);
        } catch (java.lang.reflect.InvocationTargetException ite) {
            throw (RuntimeException)ite.getTargetException();
        } catch (java.lang.IllegalAccessException iae) {
            throw new org.omg.CORBA.BAD_OPERATION();
        }
        return (org.omg.CORBA.portable.OutputStream)return_value;
    }

    public static org.omg.CORBA.portable.OutputStream __get_typeId_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        int _result = _obj.typeId();
        _output = handler.createReply();
        _output.write_long(_result);
        return _output;
    }
    public static org.omg.CORBA.portable.OutputStream __get_uid_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        java.lang.String _result = _obj.uid();
        _output = handler.createReply();
        _output.write_string(_result);
        return _output;
    }
    public static org.omg.CORBA.portable.OutputStream _propagateOnAbort_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        boolean _result = _obj.propagateOnAbort();
        _output = handler.createReply();
        _output.write_boolean(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _propagateOnCommit_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        boolean _result = _obj.propagateOnCommit();
        _output = handler.createReply();
        _output.write_boolean(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _saveRecord_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        boolean _result = _obj.saveRecord();
        _output = handler.createReply();
        _output.write_boolean(_result);
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _merge_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        _obj.merge(record);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _alter_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        _obj.alter(record);
        _output = handler.createReply();
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _shouldAdd_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        boolean _result = _obj.shouldAdd(record);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _shouldAlter_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        boolean _result = _obj.shouldAlter(record);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _shouldMerge_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        boolean _result = _obj.shouldMerge(record);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _shouldReplace_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        com.arjuna.ArjunaOTS.OTSAbstractRecordOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        com.arjuna.ArjunaOTS.OTSAbstractRecord record;
        record = com.arjuna.ArjunaOTS.OTSAbstractRecordHelper.read(_input);
        boolean _result = _obj.shouldReplace(record);
        _output = handler.createReply();
        _output.write_boolean(_result);
                return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:arjuna.com/ArjunaOTS/OTSAbstractRecord:1.0", "IDL:omg.org/CosTransactions/Resource:1.0", "IDL:omg.org/CosTransactions/SubtransactionAwareResource:1.0", "IDL:arjuna.com/ArjunaOTS/ArjunaSubtranAwareResource:1.0"};
        return __ids;
    }

    public static void _OTSAbstractRecord_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            com.arjuna.ArjunaOTS.OTSAbstractRecordOperations.class};
            try {
                _hash.put("_get_typeId", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("__get_typeId_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("_get_uid", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("__get_uid_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("propagateOnAbort", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_propagateOnAbort_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("propagateOnCommit", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_propagateOnCommit_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("saveRecord", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_saveRecord_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("merge", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_merge_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("alter", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_alter_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("shouldAdd", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_shouldAdd_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("shouldAlter", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_shouldAlter_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("shouldMerge", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_shouldMerge_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("shouldReplace", com.arjuna.ArjunaOTS.OTSAbstractRecordPOA.class.getDeclaredMethod("_shouldReplace_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _OTSAbstractRecord_hashtable;

        static
        {
            try { 
                _OTSAbstractRecord_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.ResourcePOA._Resource_init_hash(_OTSAbstractRecord_hashtable);
                org.omg.CosTransactions.SubtransactionAwareResourcePOA._SubtransactionAwareResource_init_hash(_OTSAbstractRecord_hashtable);
                com.arjuna.ArjunaOTS.OTSAbstractRecordPOA._OTSAbstractRecord_init_hash(_OTSAbstractRecord_hashtable);
                com.arjuna.ArjunaOTS.ArjunaSubtranAwareResourcePOA._ArjunaSubtranAwareResource_init_hash(_OTSAbstractRecord_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
