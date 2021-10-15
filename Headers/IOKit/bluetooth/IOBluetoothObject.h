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
/*!
 *   @header IOBluetoothObject.h
 *   This header contains the definition of the IOBluetoothObject class, a base for many classes, mostly channels, in the IOBluetoothFamily.
 */

#ifndef _IOKIT_IOBLUETOOTHOBJECT_H
#define _IOKIT_IOBLUETOOTHOBJECT_H

#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>

class IOBluetoothHCIController;

/*!
 * @class IOBluetoothObject : public IOService
 * @abstract An abstract class representing an object in the IOBluetoothFamily.
 * @discussion The IOBluetoothObject is the base for a lot of IOBluetooth classes due to its abstractness. Each IOBluetoothObject has a specific object ID, which could be obtained from the family. One of its most significant features is to provide handle pending IOs.
*/

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
    
    static IOReturn staticPerformTerminate( OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3 );
    
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
 *   Offset: 156. */
    UInt8 mTerminateMask;
    
/*! @var mBluetoothObjectID
 *   This integer records the BluetoothObjectID of this class, derived from mBluetoothFamily.
 *   Offset: 160. */
    UInt32 mBluetoothObjectID;
    
/*! @var mBluetoothFamily
 *   A  IOBluetoothHCIController that helps the class with several bluetooth-specific operations.
 *   Offset: 168. */
    IOBluetoothHCIController * mBluetoothFamily;
};

#endif
