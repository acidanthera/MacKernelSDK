/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
 * Copyright (c) 2021 cjiang.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_BLUETOOTH_IOBLUETOOTHINACTIVITYTIMEREVENTSOURCE_H
#define _IOKIT_BLUETOOTH_IOBLUETOOTHINACTIVITYTIMEREVENTSOURCE_H

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
    
    virtual bool init( OSObject * owner, Action action = NULL ) APPLE_KEXT_OVERRIDE;
    static void timerFired( Action action );
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
    virtual int getActivityCount();
    
    virtual IOReturn resetTimer();
    
protected:
    AbsoluteTime mTimeoutInterval; //96
    int mActivityCount; //104
    Action mAction; //112
};

#endif
