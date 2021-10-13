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

#ifndef _IOKIT_IOBLUETOOTHTIMEREVENTSOURCE_H
#define _IOKIT_IOBLUETOOTHTIMEREVENTSOURCE_H

#include <IOKit/IOTimerEventSource.h>

struct IOBluetoothTimer
{
    AbsoluteTime abstime; //0
    UInt32 index; //8
    void * refcon; //16
    bool enabled; //24
    IOBluetoothTimer * timerChainNext; //32
};

class IOBluetoothTimerEventSource : public IOTimerEventSource
{
    OSDeclareDefaultStructors(IOBluetoothTimerEventSource);
    
public:
    static IOBluetoothTimerEventSource * timerEventSource(OSObject * owner, Action action = NULL);
    static void timerAction( OSObject * owner, IOBluetoothTimerEventSource * sender );
    virtual void timerFired( IOBluetoothTimerEventSource * sender );
    virtual bool init(OSObject * owner, Action action = NULL) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool addTimer(AbsoluteTime abstime, UInt32 index, void * refcon);
    virtual bool removeTimer(UInt32 index);
    virtual bool removeAllTimers();
    
    virtual IOReturn setTimeoutMS(UInt32, UInt32, void *);
    virtual IOReturn setTimeoutUS(UInt32, UInt32, void *);
    virtual IOReturn setTimeout(UInt32, UInt32, UInt32, void *);
    virtual IOReturn setTimeout(AbsoluteTime abstime, UInt32, void *);
    virtual void cancelTimeout(UInt32);
    virtual void cancelTimeout() APPLE_KEXT_OVERRIDE;
    
protected:
    IOBluetoothTimer * mCurrentTimer; //96
    OSObject * mOwner; //104
    Action mTimeoutHandler; //112
    IOBluetoothTimer * mFiringTimer; //120
};

#endif
