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
/*!
 *   @header
 *   This header contains the definition of the IOBluetoothObject class, a base for many classes, such as channels, in the IOBluetoothFamily.
 */

#ifndef _IOBLUETOOTH_OBJECT_H
#define _IOBLUETOOTH_OBJECT_H

#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>

class IOBluetoothHCIController;

class IOBluetoothObject : public IOService
{
    OSDeclareAbstractStructors(IOBluetoothObject)
    
public:
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool terminate( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    virtual bool willTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn performTerminate();
    static IOReturn staticPerformTerminate(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    virtual IOCommandGate * getCommandGate() const;
    virtual IOReturn incrementNumberOfPendingIO();
    virtual IOReturn decrementNumberOfPendingIO();
    virtual UInt32 pendingIO() const;
    
protected:
    IOWorkLoop * mWorkLoop; //136
    IOCommandGate * mCommandGate; //144
    UInt32 mNumPendingIO; //152
    UInt8 mTerminateMask; //156 - the 1st bit is set to true if the class is terminated
    UInt32 mBluetoothObjectID; //160
    IOBluetoothHCIController * mBluetoothFamily; //168
};

#endif
