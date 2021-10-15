/*
 *  Released under "The GNU General Public License (GPL-2.0)"
 *
 *  Copyright (c) 2021 cjiang. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef _IOKIT_IOBLUETOOTHINACTIVITYTIMEREVENTSOURCE_H
#define _IOKIT_IOBLUETOOTHINACTIVITYTIMEREVENTSOURCE_H

#include <IOKit/IOTimerEventSource.h>

class IOBluetoothInactivityTimerEventSource : public IOTimerEventSource
{
    OSDeclareDefaultStructors(IOBluetoothInactivityTimerEventSource);
    
public:
    virtual void updateTimer();
    virtual void handleTimer( OSObject * owner );
    
    static IOBluetoothInactivityTimerEventSource * createWithTimeoutInterval( AbsoluteTime interval, OSObject * owner, Action action = NULL );
    static IOBluetoothInactivityTimerEventSource * createWithTimeoutIntervalMS( UInt32 ms, OSObject * owner, Action action = NULL );
    static IOBluetoothInactivityTimerEventSource * createWithTimeoutIntervalUS( UInt32 us, OSObject * owner, Action action = NULL );
    
/*! @function init
 *   @abstract Initializes the timer with an owner, and a handler to call when the timeout expires.
 *   @discussion This functions starts by calling IOTimerEventSource::init() with IOBluetoothInactivityTimerEventSource::timerFired as its timeout handler. Afterwards, it initializes mActivityCount to 0 and sets mTimeoutHandler to the parameter action.
 *   @param owner The owner of the timer.
 *   @param action A handler to call when the timeout expires.
 *   @result A true is returned on success and a false is returned on failure.
 */
    
    virtual bool init( OSObject * owner, Action action = NULL ) APPLE_KEXT_OVERRIDE;
    
    
    static void timerFired( OSObject * owner, IOBluetoothInactivityTimerEventSource * sender );
    virtual bool initWithTimeoutInterval( AbsoluteTime interval, OSObject * owner, Action action = NULL );
    virtual bool initWithTimeoutIntervalMS( UInt32 ms, OSObject * owner, Action action = NULL );
    virtual bool initWithTimeoutIntervalUS( UInt32 us, OSObject * owner, Action action = NULL );
    
    virtual IOReturn setTimeout( AbsoluteTime interval ) APPLE_KEXT_OVERRIDE;
    virtual void cancelTimeout() APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn setTimeoutInterval( AbsoluteTime interval );
    virtual IOReturn setTimeoutIntervalMS( UInt32 ms );
    virtual IOReturn setTimeoutIntervalUS( UInt32 us );
    virtual AbsoluteTime getTimeoutInterval();
    virtual UInt32 getTimeoutIntervalMS();
    virtual UInt32 getTimeoutIntervalUS();
    
    virtual void incrementActivityCount();
    virtual void decrementActivityCount();
    virtual UInt32 getActivityCount();
    
    virtual IOReturn resetTimer();
    
protected:
    AbsoluteTime mTimeoutInterval; //96
    UInt32 mActivityCount; //104
    Action mTimeoutHandler; //112
};

#endif
