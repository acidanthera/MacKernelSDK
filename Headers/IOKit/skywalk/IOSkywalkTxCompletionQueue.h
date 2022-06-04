/*
 * Released under "The BSD 3-Clause License"
 *
 * Copyright (c) 2022 cjiang. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The names of its contributors may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _IOSKYWALKTXCOMPLETIONQUEUE_H
#define _IOSKYWALKTXCOMPLETIONQUEUE_H

#include <IOKit/skywalk/IOSkywalkPacketQueue.h>

extern void queueNotifyAction( IOSkywalkPacketQueue * target, void * refCon );

typedef IOReturn (*IOSkywalkTxCompletionQueueAction)(OSObject * owner, IOSkywalkTxCompletionQueue *, IOSkywalkPacket **, uint, void *); // rename!!!

class IOSkywalkPacketTable;

class IOSkywalkTxCompletionQueue : public IOSkywalkPacketQueue
{
    OSDeclareDefaultStructors( IOSkywalkTxCompletionQueue )

public:
    virtual IOReturn initialize( void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual void finalize() APPLE_KEXT_OVERRIDE;
    virtual void enable() APPLE_KEXT_OVERRIDE;
    virtual void disable() APPLE_KEXT_OVERRIDE;

    virtual IOReturn performCommand( UInt32 command, void * data, size_t dataSize ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn requestEnqueue( void *, UInt32 );
    virtual IOReturn enqueuePackets( const IOSkywalkPacket ** packets, uint, IOOptionBits options );
    virtual IOReturn enqueuePackets( const queue_entry * packets, uint, IOOptionBits options );
    void completePackets();
    IOReturn enqueueAndCompletePackets(void *);
    virtual UInt32 getPacketCount() APPLE_KEXT_OVERRIDE;
    UInt32 getEffectiveCapacity(uint);
    virtual bool checkForWork() APPLE_KEXT_OVERRIDE;
    
    static IOSkywalkTxCompletionQueue * withPool( IOSkywalkPacketBufferPool * pool, UInt32 capacity, UInt32 queueId, OSObject * owner, IOSkywalkTxCompletionQueueAction action, void * refCon, IOOptionBits options );
    virtual bool initWithPool( IOSkywalkPacketBufferPool * pool, UInt32 capacity, UInt32 queueId, OSObject * owner, IOSkywalkTxCompletionQueueAction action, void * refCon, IOOptionBits options );
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    void addReporters( IOService * target, OSSet * set );
    UInt64 getReportChannelValue( UInt64 reportChannel );

    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  0 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  1 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  2 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  3 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  4 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  5 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  6 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  7 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  8 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue,  9 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxCompletionQueue, 10 );
    
protected:
    void * mReserved; // 192
    IOSkywalkPacketTable * mTable; // 200
    IOLock * mLock; // 208
    void * x_size_32; // 216
};

// size = 280

#endif
