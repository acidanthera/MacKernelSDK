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
