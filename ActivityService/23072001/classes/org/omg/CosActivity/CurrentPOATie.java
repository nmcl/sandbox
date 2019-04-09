package org.omg.CosActivity;

public class CurrentPOATie
    extends org.omg.CosActivity.CurrentPOA
{
        private org.omg.CosActivity.CurrentOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public CurrentPOATie(
            org.omg.CosActivity.CurrentOperations delegate
        )
        {
            _delegate = delegate;
        }

        public CurrentPOATie(
            org.omg.CosActivity.CurrentOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.CurrentOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.CurrentOperations delegate
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

        public void begin(
            int timeout
        ) throws org.omg.CosActivity.InvalidState, org.omg.CosActivity.TimeoutOutOfRange
        {
            _delegate.begin(timeout);
        }

        public org.omg.CosActivity.Outcome complete() throws org.omg.CosActivity.NoActivity, org.omg.CosActivity.ActivityPending, org.omg.CosActivity.ChildContextPending, org.omg.CosActivity.ActivityNotProcessed
        {
            return _delegate.complete();
        }

        public org.omg.CosActivity.Outcome complete_with_status(
            org.omg.CosActivity.CompletionStatus cs
        ) throws org.omg.CosActivity.NoActivity, org.omg.CosActivity.ActivityPending, org.omg.CosActivity.ChildContextPending, org.omg.CosActivity.InvalidState, org.omg.CosActivity.ActivityNotProcessed
        {
            return _delegate.complete_with_status(cs);
        }

        public void set_completion_status(
            org.omg.CosActivity.CompletionStatus cs
        ) throws org.omg.CosActivity.NoActivity, org.omg.CosActivity.InvalidState
        {
            _delegate.set_completion_status(cs);
        }

        public org.omg.CosActivity.CompletionStatus get_completion_status() throws org.omg.CosActivity.NoActivity
        {
            return _delegate.get_completion_status();
        }

        public void set_completion_signal_set(
            java.lang.String signal_set_name
        ) throws org.omg.CosActivity.NoActivity, org.omg.CosActivity.SignalSetUnknown, org.omg.CosActivity.InvalidState
        {
            _delegate.set_completion_signal_set(signal_set_name);
        }

        public java.lang.String get_completion_signal_set() throws org.omg.CosActivity.NoActivity
        {
            return _delegate.get_completion_signal_set();
        }

        public org.omg.CosActivity.ActivityToken suspend() throws org.omg.CosActivity.InvalidParentContext
        {
            return _delegate.suspend();
        }

        public void resume(
            org.omg.CosActivity.ActivityToken at
        ) throws org.omg.CosActivity.InvalidToken, org.omg.CosActivity.InvalidParentContext
        {
            _delegate.resume(at);
        }

        public org.omg.CosActivity.ActivityToken suspend_all()
        {
            return _delegate.suspend_all();
        }

        public void resume_all(
            org.omg.CosActivity.ActivityToken at
        ) throws org.omg.CosActivity.InvalidToken, org.omg.CosActivity.InvalidParentContext
        {
            _delegate.resume_all(at);
        }

        public byte[] get_global_id()
        {
            return _delegate.get_global_id();
        }

        public org.omg.CosActivity.Status get_status()
        {
            return _delegate.get_status();
        }

        public java.lang.String get_activity_name()
        {
            return _delegate.get_activity_name();
        }

        public void set_timeout(
            int seconds
        ) throws org.omg.CosActivity.TimeoutOutOfRange
        {
            _delegate.set_timeout(seconds);
        }

        public int get_timeout()
        {
            return _delegate.get_timeout();
        }

        public org.omg.CosActivity.ActivityContext get_context()
        {
            return _delegate.get_context();
        }

        public void recreate_context(
            org.omg.CosActivity.ActivityContext ctx
        ) throws org.omg.CosActivity.InvalidContext
        {
            _delegate.recreate_context(ctx);
        }

        public org.omg.CosActivity.ActivityCoordinator get_coordinator()
        {
            return _delegate.get_coordinator();
        }

        public org.omg.CosActivity.ActivityCoordinator get_parent_coordinator()
        {
            return _delegate.get_parent_coordinator();
        }

        public org.omg.CosActivity.ActivityIdentity get_identity()
        {
            return _delegate.get_identity();
        }

        public org.omg.CosActivity.ActivityToken get_token()
        {
            return _delegate.get_token();
        }

        public org.omg.CosActivity.PropertyGroup get_property_group(
            java.lang.String name
        ) throws org.omg.CosActivity.PropertyGroupUnknown, org.omg.CosActivity.NoActivity
        {
            return _delegate.get_property_group(name);
        }

}
