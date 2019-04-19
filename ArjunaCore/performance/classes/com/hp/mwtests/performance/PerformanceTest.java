package com.hp.mwtests.performance;

public abstract class PerformanceTest
{
    private String[]    _configs = null;
    private String[]    _params = null;

    protected abstract void work() throws Exception;

    public void setServiceConfigs( String[] configs )
    {
        _configs = configs;
    }

    void setParameters(String[] params)
    {
        _params = params;
    }

    public boolean isParameterDefined(String param)
    {
        for (int count=0;count<_params.length;count++)
        {
            if ( param.equals( _params[count] ) )
            {
                return true;
            }
        }

        return false;
    }

    public String[] getParameters()
    {
        return _params;
    }

    public String getServiceConfig( int index )
    {
        return _configs[index];
    }

    public void performWork( int numberOfIterations ) throws Exception
    {
        for (int count=0;count<numberOfIterations;count++)
        {
            work();
        }
    }
}
