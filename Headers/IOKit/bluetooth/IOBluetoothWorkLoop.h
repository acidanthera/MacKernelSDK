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
 *   @header IOBluetoothWorkLoop.h
 *   This header contains the definitions of the IOBluetoothLocalUtilityEventSource and IOBluetoothWorkLoop classes.
 */

#ifndef _IOKIT_IOBLUETOOTHWORKLOOP_H
#define _IOKIT_IOBLUETOOTHWORKLOOP_H

#include <IOKit/IOEventSource.h>
#include <IOKit/IOWorkLoop.h>

/*!
 * @class IOBluetoothLocalUtilityEventSource : public IOEventSource
 * @abstract An event source providing utilities specific to the IOBluetooth family.
 * @discussion The IOBluetoothLocalUtilityEventSource expands the functionality of its super class, IOEventSource, by implementing the functions closeGateOn() and openGateOn(), as well as a factory member function for organized construction of itself. This event source should be used and only used in the IOBluetooth family.
*/

class IOBluetoothLocalUtilityEventSource : public IOEventSource
{
    OSDeclareDefaultStructors(IOBluetoothLocalUtilityEventSource)
    
public:
/*! @function ioBluetoothLocalUtilityEventSource
 *   @abstract Factory member function to constuct and intialize an IOBluetoothLocalUtilityEventSource.
 *   @result Returns a IOBluetoothLocalUtilityEventSource instance if constructed successfully, 0 otherwise.
*/
    
    static IOBluetoothLocalUtilityEventSource * ioBluetoothLocalUtilityEventSource();
    
/*! @function checkForWork
 *    @abstract Virtual member function used by IOWorkLoop for work scheduling.
 *    @discussion This function overrides IOEventSource::checkForWork() so that it does nothing.
 *    @result Returns false.
*/
    
    virtual bool checkForWork() APPLE_KEXT_OVERRIDE;
    
/*! @function closeGateOn
 *   @abstract Closes gate and sets the work loop.
 *   @discussion This function calls IOEventSource::setWorkLoop() and then IOEventSource::closeGate().
 *   @param workLoop The parameter used in IOEventSource::setWorkLoop(). */
    
    virtual void closeGateOn( IOWorkLoop * workLoop );
    
/*! @function openGateOn
 *   @abstract Opens gate and sets the work loop.
 *   @discussion This function calls IOEventSource::setWorkLoop() and then IOEventSource::openGate().
 *   @param workLoop The parameter used in IOEventSource::setWorkLoop(). */
    
    virtual void openGateOn( IOWorkLoop * workLoop );
};

/*!
 * @class IOBluetoothWorkLoop : public IOWorkLoop
 * @abstract A work loop instance providing functions used exclusively by the IOBluetooth family.
 * @discussion The IOBluetoothWorkLoop provides the functions handoffFrom() and returnTo(), which could help with linking between different work loops, a feature that could be used by the IOBluetoothFamily. This class also contains a factory member function for organized construction of itself. This work loop should be used and only used in the IOBluetooth family.
*/

class IOBluetoothWorkLoop : public IOWorkLoop
{
    OSDeclareDefaultStructors(IOBluetoothWorkLoop)
    
public:
/*! @function workLoop
 *   @abstract Factory member function to construct and intialize an IOBluetoothWorkLoop.
 *   @result Returns a IOBluetoothWorkLoop instance if constructed successfully, 0 otherwise.
*/
    
    static IOBluetoothWorkLoop * workLoop();
    
/*! @function handoffFrom
 *   @abstract Handoffs from a work loop instance.
 *   @discussion This function will first check if workLoop is inGate(). Then, it calls openGateOn(workLoop) using an IOBluetoothLocalUtilityEventSource instance and closeGate() on this.
 *   @param workLoop A work loop instance.
 *   @result An IOReturn indicating whether the operation is successful.
*/
    
    virtual IOReturn handoffFrom(IOWorkLoop * workLoop);
    
/*! @function returnTo
 *   @abstract Returns to a work loop instance.
 *   @discussion This function will first check if workLoop is inGate(). Then, it calls openGate() on this and closeGateOn(workLoop) using an IOBluetoothLocalUtilityEventSource instance.
 *   @param workLoop A work loop instance.
 *   @result An IOReturn indicating whether the operation is successful.
*/
    
    virtual IOReturn returnTo(IOWorkLoop * workLoop);
};

#endif
