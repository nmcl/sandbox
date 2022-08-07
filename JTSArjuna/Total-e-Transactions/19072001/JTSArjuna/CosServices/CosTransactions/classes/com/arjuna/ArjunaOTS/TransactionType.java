//
// Java code automatically generated by the Orbix 2000 IDL Compiler
// Do not edit this code!
//

package com.arjuna.ArjunaOTS;
public class TransactionType
    implements org.omg.CORBA.portable.IDLEntity
{
    public static final int _TransactionTypeActive = 0;
    public static final TransactionType TransactionTypeActive = new TransactionType(0);
    public static final int _TransactionTypeTerminated = 1;
    public static final TransactionType TransactionTypeTerminated = new TransactionType(1);
    public static final int _TransactionTypeUnresolved = 2;
    public static final TransactionType TransactionTypeUnresolved = new TransactionType(2);

    public static final TransactionType IT_ENUM_MAX = new TransactionType(Integer.MAX_VALUE);

    public static TransactionType from_int(
        int value
    )
    {
        switch (value)
        {
            case _TransactionTypeActive: return TransactionTypeActive;

            case _TransactionTypeTerminated: return TransactionTypeTerminated;

            case _TransactionTypeUnresolved: return TransactionTypeUnresolved;

            default:
                 return IT_ENUM_MAX;
        }
    }

    private int ___value;

    public int value()
    {
        return ___value;
    }

    private TransactionType(
        int value
    )
    {
        ___value = value;
    }

    public java.lang.Object clone()
    {
        return from_int(___value);
    }
}
