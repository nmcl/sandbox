/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseHLS.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.hls;

import com.arjuna.mw.wsas.activity.HLS;
import com.arjuna.mw.wscf.model.twophase.api.*;

import com.arjuna.mw.wscf.api.UserCoordinatorService;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TwoPhaseHLS.java,v 1.1 2003/01/07 10:33:45 nmcl Exp $
 * @since 1.0.
 */

public interface TwoPhaseHLS extends HLS
{
    
    public UserCoordinatorService coordinatorService ();
    
    public UserCoordinator userCoordinator ();
    
    public CoordinatorManager coordinatorManager ();

}

