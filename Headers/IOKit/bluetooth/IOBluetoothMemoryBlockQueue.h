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
 *   @header IOBluetoothMemoryBlockQueue.h
 *   This header contains the definition of the IOBluetoothMemoryBlockQueue instance.
 */

#ifndef _IOKIT_BLUETOOTH_IOBLUETOOTHMEMORYBLOCKQUEUE_H
#define _IOKIT_BLUETOOTH_IOBLUETOOTHMEMORYBLOCKQUEUE_H

class IOBluetoothMemoryBlock;

struct IOBluetoothMemoryBlockQueueNode
{
    IOBluetoothMemoryBlock * block;
    IOBluetoothMemoryBlockQueueNode * next;
    bool completionCalled;
};

class IOBluetoothMemoryBlockQueue : public OSObject
{
    OSDeclareDefaultStructors(IOBluetoothMemoryBlockQueue)
    
public:
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    
/*! @function queueIsEmpty
 *   @abstract Determines if the queue is empty.
 *   @discussion The queue would be counted as empty if mHeadNode is NULL and mNumEntriesInQueue is 0.
 *   @result If the queue is empty, the result would be true. Otherwise, it would be false. */
    
    virtual bool queueIsEmpty();
    
/*! @function enqueueBlock
 *   @abstract Adds a new memory block to the tail of the queue.
 *   @discussion This function adopts the same approach of enqueuing an object as any other queue, but it is not done directly. The block is to be placed into an IOBluetoothMemoryBlockQueueNode, which will instead be enqueued. If the function is successfully executed, mNumEntriesInQueue would increment.
 *   @param block The memory block to enqueue.
 *   @result An IOReturn indicating whether the operation was successful. */
    
    virtual IOReturn enqueueBlock(IOBluetoothMemoryBlock * block);
    
/*! @function peekNextBlock
 *   @abstract Derives the memory block at the head of the queue.
 *   @result The memory block of mHeadNode. If it does not exist, NULL will be returned. */
    
    virtual IOBluetoothMemoryBlock * peekNextBlock();
    
/*! @function enqueueBlock
 *   @abstract Removes the memory block at the head of the queue.
 *   @discussion If the queue is not empty, this function will remove mHeadNode by releasing it and moving the next entry to its address. If the function is successfully executed, mNumEntriesInQueue would decrement.
 *   @result The IOBluetoothMemoryBlock that is in the entry dequeued. If nothing is dequeued, it would be NULL. */
    
    virtual IOBluetoothMemoryBlock * dequeueBlock();
    
/*! @function flushQueue
 *   @abstract Removes all entries from the queue.
 *   @discussion This functions continuously calls dequeueBlock() until there are no more entries left.
 *   @param releaseBlock If this parameter is true, the IOBluetoothMemoryBlock instances in the entries dequeued will be released. Vice versa.*/
    
    virtual void flushQueue(bool releaseBlock);
    virtual UInt32 numberOfEntriesInQueue();
    virtual IOReturn setCompletionCalled(IOBluetoothMemoryBlock * block);
    virtual bool completionRoutineIsCalled();
    virtual IOReturn removeBlock(IOBluetoothMemoryBlock * block);
    virtual void printQueue();
    
protected:
    IOBluetoothMemoryBlockQueueNode * mHeadNode; //16
    IOBluetoothMemoryBlockQueueNode * mTailNode; //24
    UInt32 mNumEntriesInQueue; //32
};

#endif
