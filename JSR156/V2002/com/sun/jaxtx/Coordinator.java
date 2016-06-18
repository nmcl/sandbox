package com.sun.jaxtx;

/*
 * This class represents the remotely invocable portion of the
 * coordinator.
 */

public class Coordinator
{

    public String complete_transaction (int cs)
    {
	return null;
    }

    public String coordinate (int cs)
    {
    }
    
    public void send_outcome (String document)
    {
    }
    
    public String get_outcome ()
    {
	return null;
    }
    
    public int add_participant (Participant act, int priority)
    {
	return 0;
    }
    
    public void remove_participant (Participant act)
    {
    }
    
    public int set_response (String id, String response)
    {
	return 0;
    }
    
    public Coordinator get_parent_coordinator ()
    {
	return null;
    }

    public String get_transaction_id ()
    {
	return null;
    }

    public int get_status ()
    {
	return 0;
    }
    
    public int get_parent_status ()
    {
	return 0;
    }
    
    public String get_transaction_name ()
    {
	return null;
    }

    public boolean is_same_transaction (Coordinator ac)
    {
	return false;
    }

}
