package com.example.helloandroid;

import java.io.IOException;

import com.arjuna.ats.arjuna.AtomicAction;
import com.arjuna.ats.arjuna.ObjectModel;
import com.arjuna.ats.arjuna.ObjectType;
import com.arjuna.ats.arjuna.common.Uid;
import com.arjuna.ats.arjuna.coordinator.ActionStatus;
import com.arjuna.ats.arjuna.state.InputObjectState;
import com.arjuna.ats.arjuna.state.OutputObjectState;
import com.arjuna.ats.txoj.Lock;
import com.arjuna.ats.txoj.LockManager;
import com.arjuna.ats.txoj.LockMode;
import com.arjuna.ats.txoj.LockResult;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

class AtomicObject extends LockManager
{
    public AtomicObject()
    {
        this(ObjectModel.SINGLE);
    }

    public AtomicObject(int om)
    {
        super(ObjectType.ANDPERSISTENT, om);

        state = 0;

        AtomicAction A = new AtomicAction();

        A.begin();

        if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
        {
            if (A.commit() == ActionStatus.COMMITTED)
                System.out.println("Created persistent object " + get_uid());
            else
                System.out.println("Action.commit error.");
        }
        else
        {
            A.abort();

            System.out.println("setlock error.");
        }
    }

    public AtomicObject(Uid u)
    {
        this(u, ObjectModel.SINGLE);
    }

    public AtomicObject(Uid u, int om)
    {
        super(u, ObjectType.ANDPERSISTENT, om);

        state = -1;

        AtomicAction A = new AtomicAction();

        A.begin();

        if (setlock(new Lock(LockMode.READ), 0) == LockResult.GRANTED)
        {
            System.out.println("Recreated object " + u);
            A.commit();
        }
        else
        {
            System.out.println("Error recreating object " + u);
            A.abort();
        }
    }

    public int getRetry ()
    {
        return retry;
    }
    
    public void setRetry (int t)
    {
        retry = t;
    }
    
    public void terminate ()
    {
        super.terminate();
    }
    
    public void incr (int value)
    {
        AtomicAction A = new AtomicAction();

        A.begin();

        if (setlock(new Lock(LockMode.WRITE), retry) == LockResult.GRANTED)
        {
            state += value;

            if (A.commit() != ActionStatus.COMMITTED)
                System.err.println("Action commit error.");
            else
                return;
        }

        A.abort();
    }

    public void set (int value)
    {
        AtomicAction A = new AtomicAction();

        A.begin();

        if (setlock(new Lock(LockMode.WRITE), retry) == LockResult.GRANTED)
        {
            state = value;

            if (A.commit() != ActionStatus.COMMITTED)
                System.err.println("Action commit error.");
            else
                return;
        }

        A.abort();
    }

    public int get ()
    {
        AtomicAction A = new AtomicAction();
        int value = -1;

        A.begin();

        if (setlock(new Lock(LockMode.READ), retry) == LockResult.GRANTED)
        {
            value = state;

            if (A.commit() == ActionStatus.COMMITTED)
                return value;
            else
                System.err.println("Action commit error.");
        }

        A.abort();
        
        return -1;
    }

    public boolean save_state (OutputObjectState os, int ot)
    {
        boolean result = super.save_state(os, ot);

        if (!result)
            return false;

        try
        {
            os.packInt(state);
        }
        catch (IOException e)
        {
            result = false;
        }

        return result;
    }

    public boolean restore_state (InputObjectState os, int ot)
    {
        boolean result = super.restore_state(os, ot);

        if (!result)
            return false;

        try
        {
            state = os.unpackInt();
        }
        catch (IOException e)
        {
            result = false;
        }

        return result;
    }

    public String type ()
    {
        return "/StateManager/LockManager/AtomicObject";
    }

    private int state;
    private int retry = 0;
}

public class HelloAndroid extends Activity {
   /** Called when the activity is first created. */
   @Override
   public void onCreate(Bundle savedInstanceState) {
       
       super.onCreate(savedInstanceState);
       TextView tv = new TextView(this);

       AtomicAction act = new AtomicAction();
       
       act.begin();
       
       act.abort();
       
       //tv.setText("Hello, TxAndroid created "+act.toString());
 
       AtomicAction A = new AtomicAction();
       AtomicObject foo = new AtomicObject();
       
           A.begin();

           foo.set(2);

           A.commit();
       
           tv.setText("Hello, TxAndroid created "+A.toString()+" and got "+foo.get());
           
       setContentView(tv);
   }
}