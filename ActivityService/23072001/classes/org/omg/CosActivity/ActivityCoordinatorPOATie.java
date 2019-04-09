package org.omg.CosActivity;

public class ActivityCoordinatorPOATie
    extends org.omg.CosActivity.ActivityCoordinatorPOA
{
        private org.omg.CosActivity.ActivityCoordinatorOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public ActivityCoordinatorPOATie(
            org.omg.CosActivity.ActivityCoordinatorOperations delegate
        )
        {
            _delegate = delegate;
        }

        public ActivityCoordinatorPOATie(
            org.omg.CosActivity.ActivityCoordinatorOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.ActivityCoordinatorOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.ActivityCoordinatorOperations delegate
        )
        {
            _delegate = delegate;
        }

        public org.omg.PortableServer.POA _default_POA()
        {
            if (_poa != null)
            {
                return _poa;
            }

            else
            {
                return super._default_POA();
            }

        }

        public org.omg.CosActivity.Outcome complete_activity(
            java.lang.String signal_set_name,
            org.omg.CosActivity.CompletionStatus cs
        ) throws org.omg.CosActivity.ActivityPending, org.omg.CosActivity.ChildContextPending, org.omg.CosActivity.SignalSetUnknown, org.omg.CosActivity.ActivityNotProcessed
        {
            return _delegate.complete_activity(signal_set_name, cs);
        }

        public org.omg.CosActivity.Outcome process_signal_set(
            java.lang.String signal_set_name,
            org.omg.CosActivity.CompletionStatus cs
        ) throws org.omg.CosActivity.SignalSetUnknown, org.omg.CosActivity.ActivityNotProcessed
        {
            return _delegate.process_signal_set(signal_set_name, cs);
        }

        public void add_signal_set(
            org.omg.CosActivity.SignalSet signal_set
        ) throws org.omg.CosActivity.SignalSetAlreadyRegistered
        {
            _delegate.add_signal_set(signal_set);
        }

        public void remove_signal_set(
            java.lang.String signal_set_name
        ) throws org.omg.CosActivity.SignalSetUnknown
        {
            _delegate.remove_signal_set(signal_set_name);
        }

        public void add_action(
            org.omg.CosActivity.Action act,
            java.lang.String signal_set_name,
            int priority
        ) throws org.omg.CosActivity.SignalSetUnknown
        {
            _delegate.add_action(act, signal_set_name, priority);
        }

        public void remove_action(
            org.omg.CosActivity.Action act
        ) throws org.omg.CosActivity.ActionNotFound
        {
            _delegate.remove_action(act);
        }

        public void add_actions(
            org.omg.CosActivity.Action[] acts,
            java.lang.String signal_set_name,
            int priority
        ) throws org.omg.CosActivity.SignalSetUnknown
        {
            _delegate.add_actions(acts, signal_set_name, priority);
        }

        public org.omg.CosActivity.Action[] remove_actions(
            org.omg.CosActivity.Action[] acts
        )
        {
            return _delegate.remove_actions(acts);
        }

        public void add_global_action(
            org.omg.CosActivity.Action act,
            int priority
        )
        {
            _delegate.add_global_action(act, priority);
        }

        public void remove_global_action(
            org.omg.CosActivity.Action act
        ) throws org.omg.CosActivity.ActionNotFound
        {
            _delegate.remove_global_action(act);
        }

        public int get_number_registered_actions(
            java.lang.String signal_set_name
        ) throws org.omg.CosActivity.SignalSetUnknown
        {
            return _delegate.get_number_registered_actions(signal_set_name);
        }

        public org.omg.CosActivity.Action[] get_actions(
            java.lang.String signal_set_name
        ) throws org.omg.CosActivity.SignalSetUnknown
        {
            return _delegate.get_actions(signal_set_name);
        }

        public org.omg.CosActivity.ActivityCoordinator get_parent_coordinator()
        {
            return _delegate.get_parent_coordinator();
        }

        public byte[] get_global_id()
        {
            return _delegate.get_global_id();
        }

        public org.omg.CosActivity.Status get_status()
        {
            return _delegate.get_status();
        }

        public org.omg.CosActivity.Status get_parent_status()
        {
            return _delegate.get_parent_status();
        }

        public java.lang.String get_activity_name()
        {
            return _delegate.get_activity_name();
        }

        public boolean is_same_activity(
            org.omg.CosActivity.ActivityCoordinator ac
        )
        {
            return _delegate.is_same_activity(ac);
        }

        public int hash_activity()
        {
            return _delegate.hash_activity();
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
