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

#ifndef _IOKIT_IOBLUETOOTHDATAQUEUE_H
#define _IOKIT_IOBLUETOOTHDATAQUEUE_H

#include <IOKit/IOSharedDataQueue.h>
#include <IOKit/IOMemoryDescriptor.h>

#ifdef enqueue
#undef enqueue
#endif

/*!
 * @class IOBluetoothDataQueue : public IOSharedDataQueue
 * @abstract A generic data queue providing utilities specific to the IOBluetoothFamily.
 * @discussion The IOBluetoothDataQueue is a subclass of IOSharedDataQueue that provides some functions that would be used by other IOBluetoothFamily classes. It should be used and only used in this family.
*/

class IOBluetoothDataQueue : public IOSharedDataQueue
{
    OSDeclareDefaultStructors(IOBluetoothDataQueue)

public:
    static IOBluetoothDataQueue * withCapacity(UInt32 size);
    static IOBluetoothDataQueue * withEntries(UInt32 numEntries, UInt32 entrySize);
    static IOBluetoothDataQueue * withBuffer(void * buffer, UInt32 size);
    static IOBluetoothDataQueue * withClientBuffer(mach_vm_address_t address, UInt32 length, task_t task);
    
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual void sendDataAvailableNotification() APPLE_KEXT_OVERRIDE;
    virtual void setNotificationPort(mach_port_t port) APPLE_KEXT_OVERRIDE;
    virtual bool setQueueSize(UInt32 size, bool isWithBuffer);
    virtual bool initWithCapacity(UInt32 size) APPLE_KEXT_OVERRIDE;
    virtual bool initWithBuffer(void * buffer, UInt32 size);
    virtual bool initWithClientBuffer(mach_vm_address_t address, UInt32 length, task_t task);
    virtual bool enqueue(void * data, UInt32 dataSize) APPLE_KEXT_OVERRIDE;
    virtual UInt32 numberOfFreeBlocks(UInt64 offset);
    virtual UInt32 freeSpaceInQueue();
    virtual void setDataQueueOwnerName(char * name);
    
protected:
    bool mInitializedWithBuffer; //40, determines if the data queue should be freed
    char mDataQueueOwnerName[0x30]; //41
    IOMemoryMap * mMemoryMap; //96
    IOMemoryDescriptor * mMemoryDescriptor; //104
    bool mInitializedWithClientBuffer; //112, will setTag(3) in free if it is true, set to true in initWithClientBuffer
    UInt32 mNotifyMsgSize; //116
};

#endif
