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
