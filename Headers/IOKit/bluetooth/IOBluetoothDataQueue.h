/*
* Copyright © 1998-2021 Apple Inc. All rights reserved.
* Copyright © 2021 cjiang. All rights reserved.
*
* @APPLE_LICENSE_HEADER_START@
*
* This file contains Original Code and/or Modifications of Original Code
* as defined in and that are subject to the Apple Public Source License
* Version 2.0 (the 'License'). You may not use this file except in
* compliance with the License. Please obtain a copy of the License at
* http://www.opensource.apple.com/apsl/ and read it before using this
* file.
*
* The Original Code and all software distributed under the License are
* distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
* EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
* Please see the License for the specific language governing rights and
* limitations under the License.
*
* @APPLE_LICENSE_HEADER_END@
*/

#ifndef _IOBLUETOOTH_DATAQUEUE_H
#define _IOBLUETOOTH_DATAQUEUE_H

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
    char mDataQueueOwnerName[50]; //41
    IOMemoryMap * mMemoryMap; //96
    IOMemoryDescriptor * mMemoryDescriptor; //104
    bool mInitializedWithClientBuffer; //112, will setTag(3) in free if it is true, set to true in initWithClientBuffer
    UInt32 mNotifyMsgSize; //116
};

#endif
