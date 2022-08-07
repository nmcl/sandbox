package org.omg.CosTransactions;

abstract public class TerminatorPOA
    extends org.omg.PortableServer.Servant
    implements org.omg.CORBA.portable.InvokeHandler, TerminatorOperations
{
    public org.omg.CosTransactions.Terminator _this()
    {
        return org.omg.CosTransactions.TerminatorHelper.narrow(_this_object());
    }

    public org.omg.CosTransactions.Terminator _this(
        org.omg.CORBA.ORB orb
)
    {
        return org.omg.CosTransactions.TerminatorHelper.narrow(_this_object(orb));
    }

    public org.omg.CORBA.portable.OutputStream _invoke(String opName, 
        org.omg.CORBA.portable.InputStream input, 
        org.omg.CORBA.portable.ResponseHandler handler    )
    {
        java.lang.Object[] _params = {input, handler, this};
        java.lang.reflect.Method _meth = (java.lang.reflect.Method)_Terminator_hashtable.get(opName);
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

    public static org.omg.CORBA.portable.OutputStream _commit_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.TerminatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        try {
            boolean report_heuristics;
            report_heuristics = _input.read_boolean();
            _obj.commit(report_heuristics);
            _output = handler.createReply();
                        }
        catch (org.omg.CosTransactions.HeuristicMixed _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicMixedHelper.write(_output, _exception0);
            return _output;
        }
        catch (org.omg.CosTransactions.HeuristicHazard _exception0)
        {
            _output = handler.createExceptionReply();
            org.omg.CosTransactions.HeuristicHazardHelper.write(_output, _exception0);
            return _output;
        }
        return _output;
    }


    public static org.omg.CORBA.portable.OutputStream _rollback_static(
        org.omg.CORBA.portable.InputStream _input,
        org.omg.CORBA.portable.ResponseHandler handler, 
        org.omg.CosTransactions.TerminatorOperations _obj 
    )
    {
        org.omg.CORBA.portable.OutputStream _output = null;
        _obj.rollback();
        _output = handler.createReply();
        return _output;
    }


    public String[]  _all_interfaces(
        org.omg.PortableServer.POA poa,
        byte[] objectId
    )
    {
        String __ids[] =  {"IDL:omg.org/CosTransactions/Terminator:1.0"};
        return __ids;
    }

    public static void _Terminator_init_hash(
        java.util.HashMap _hash
    )
    {
        java.lang.Class[] params = {org.omg.CORBA.portable.InputStream.class,
            org.omg.CORBA.portable.ResponseHandler.class,
            org.omg.CosTransactions.TerminatorOperations.class};
            try {
                _hash.put("commit", org.omg.CosTransactions.TerminatorPOA.class.getDeclaredMethod("_commit_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
            try {
                _hash.put("rollback", org.omg.CosTransactions.TerminatorPOA.class.getDeclaredMethod("_rollback_static", params));
            } catch (java.lang.NoSuchMethodException nsme) {
                throw new org.omg.CORBA.BAD_OPERATION();
            }
        }

        private static java.util.HashMap _Terminator_hashtable;

        static
        {
            try { 
                _Terminator_hashtable = new java.util.HashMap();
                org.omg.CosTransactions.TerminatorPOA._Terminator_init_hash(_Terminator_hashtable);
            } catch (java.lang.Exception excep) {
                excep.printStackTrace();
            }
        }

}
