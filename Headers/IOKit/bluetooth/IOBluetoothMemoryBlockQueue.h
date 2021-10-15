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
 *   @header IOBluetoothMemoryBlockQueue.h
 *   This header contains the definition of the IOBluetoothMemoryBlockQueue instance.
 */

#ifndef _IOKIT_IOBLUETOOTHMEMORYBLOCKQUEUE_H
#define _IOKIT_IOBLUETOOTHMEMORYBLOCKQUEUE_H

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
