package org.omg.CosActivity;

public class SignalSetPOATie
    extends org.omg.CosActivity.SignalSetPOA
{
        private org.omg.CosActivity.SignalSetOperations _delegate;
        private org.omg.PortableServer.POA _poa;

        public SignalSetPOATie(
            org.omg.CosActivity.SignalSetOperations delegate
        )
        {
            _delegate = delegate;
        }

        public SignalSetPOATie(
            org.omg.CosActivity.SignalSetOperations delegate,
            org.omg.PortableServer.POA poa
        )
        {
            _delegate = delegate;
            _poa = poa;
        }

        public org.omg.CosActivity.SignalSetOperations _delegate()
        {
            return _delegate;
        }

        public void _delegate(
            org.omg.CosActivity.SignalSetOperations delegate
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

        public java.lang.String signal_set_name()
        {
            return _delegate.signal_set_name();
        }

        public org.omg.CosActivity.Signal get_signal(
            org.omg.CORBA.BooleanHolder lastSignal
        )
        {
            return _delegate.get_signal(lastSignal);
        }

        public org.omg.CosActivity.Outcome get_outcome() throws org.omg.CosActivity.SignalSetActive
        {
            return _delegate.get_outcome();
        }

        public boolean set_response(
            org.omg.CosActivity.Outcome response,
            org.omg.CORBA.BooleanHolder nextSignal
        ) throws org.omg.CosActivity.SignalSetInactive
        {
            return _delegate.set_response(response, nextSignal);
        }

        public void set_completion_status(
            org.omg.CosActivity.CompletionStatus cs
        )
        {
            _delegate.set_completion_status(cs);
        }

        public org.omg.CosActivity.CompletionStatus get_completion_status()
        {
            return _delegate.get_completion_status();
        }

        public void set_activity_coordinator(
            org.omg.CosActivity.ActivityCoordinator coord
        ) throws org.omg.CosActivity.SignalSetActive
        {
            _delegate.set_activity_coordinator(coord);
        }

        public void destroy() throws org.omg.CosActivity.AlreadyDestroyed
        {
            _delegate.destroy();
        }

}
