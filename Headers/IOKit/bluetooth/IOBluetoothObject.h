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
    
/*! @function willTerminate
 *   @abstract Passes a termination up the stack.
 *   @discussion Retains and releases the class per se, runs staticPerformTerminate from the command gate, and calls IOService::willTerminate().
 *   @param provider The terminated provider of this object.
 *   @param options Options originally passed to terminate.
 *   @result An boolean indicating whether the operation was successful. */
    
    virtual bool willTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    
/*! @function performTerminate
 *   @abstract Use the command gate to sleep all pending IOs.
 *   @result For now, this function always returns kIOReturnSuccess. */
    
    virtual IOReturn performTerminate();
    
/*! @function staticPerformTerminate
 *   @abstract Static version of performTerminate, implemented by converting owner to an IOBluetoothObject and calling performTerminate from there.
 *   @param owner The function's provider that will be converted to an IOBluetoothObject.
 *   @param arg0 Argument to action from run operation.
 *   @param arg1 Argument to action from run operation.
 *   @param arg2 Argument to action from run operation.
 *   @param arg3 Argument to action from run operation.
 *   @result An IOReturn indicating whether the operation was successful. */
    
    static IOReturn staticPerformTerminate( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    
/*! @function getWorkLoop
 *   @abstract Returns the current work loop.
 *   @result The current work loop, specifically mWorkLoop. */
    
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    
/*! @function getCommandGate
 *   @abstract Returns the current command gate.
 *   @result The current command gate, specifically mCommandGate. */
    
    virtual IOCommandGate * getCommandGate() const;
    
/*! @function incrementNumberOfPendingIO
 *   @abstract Increments the number of pending IOs by 1.
 *   @discussion This function would increment mNumPendingIO by 1 only if the workLoop is inGate() and the integer is within the range of an UInt32.
 *   @result An IOReturn indicating whether the operation was successful. */
    
    virtual IOReturn incrementNumberOfPendingIO();
    
/*! @function decrementNumberOfPendingIO
 *   @abstract Decrements the number of pending IOs by 1.
 *   @discussion This function would decrement mNumPendingIO by 1 only if the workLoop is inGate() and the integer is within the range of an UInt32. If there are no pending IOs left after the decrement, it would call commandWakeup() to wakeup the threads.
 *   @result An IOReturn indicating whether the operation was successful. */
    
    virtual IOReturn decrementNumberOfPendingIO();
    
/*! @function pendingIO
 *   @abstract Returns the number of attached pending IOs.
 *   @result The current number of pending IOs, specifically mNumPendingIO. */
    
    virtual UInt32 pendingIO() const;
    
protected:
/*! @var mWorkLoop
 *   The work loop of this class. For now, mCommandGate is the only event source that will be added.
 *   Offset: 136. */
    IOWorkLoop * mWorkLoop;
    
/*! @var mWorkLoop
 *   The command gate of this class.
 *   Offset: 144. */
    IOCommandGate * mCommandGate;
    
/*! @var mNumPendingIO
 *   This integer records to number of attached pending IOs. It could be obtained through pendingIO().
 *   Offset: 152. */
    UInt32 mNumPendingIO;
    
/*! @var mTerminateMask
 *   This unsigned char is used to mark if the class has been terminated. The second bit is set to true when termiante() is called.
 *   It's offset is 156. */
    UInt8 mTerminateMask;
    
/*! @var mTerminateMask
 *   This integer records the BluetoothObjectID of this class, derived from mBluetoothFamily.
 *   It's offset is 160. */
    UInt32 mBluetoothObjectID;
    
/*! @var mBluetoothFamily
 *   A  IOBluetoothHCIController that helps the class with several bluetooth-specific operations.
 *   It's offset is 168. */
    IOBluetoothHCIController * mBluetoothFamily;
};

#endif
