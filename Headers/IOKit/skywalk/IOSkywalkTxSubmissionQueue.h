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

#ifndef _IOSKYWALKTXSUBMISSIONQUEUE_H
#define _IOSKYWALKTXSUBMISSIONQUEUE_H

#include <IOKit/skywalk/IOSkywalkPacketQueue.h>

class IOSkywalkTxSubmissionQueue : public IOSkywalkPacketQueue
{
    OSDeclareDefaultStructors( IOSkywalkTxSubmissionQueue )

public:
    
    void addReporters( IOService * target, OSSet * set );
    UInt64 getReportChannelValue( UInt64 reportChannel );

    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  0 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  1 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  2 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  3 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  4 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  5 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  6 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  7 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  8 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue,  9 );
    OSMetaClassDeclareReservedUnused( IOSkywalkTxSubmissionQueue, 10 );

protected:
    virtual IOReturn initialize( void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual void finalize() APPLE_KEXT_OVERRIDE;
    virtual void enable() APPLE_KEXT_OVERRIDE;
    virtual void disable() APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn synchronize( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    void requestAsyncRefill();
    void refillTxRing( bool txDoorbellContext );
    
    virtual IOReturn sendNotification( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn handleDoorbell( IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    void gatedDequeue( void * refCon );
    virtual IOReturn requestDequeue( void * refCon, IOOptionBits options );
    virtual void purgePackets();

    void adjustPacketCounters();
    virtual UInt32 getPacketCount() APPLE_KEXT_OVERRIDE;
    virtual IOReturn getDataByteCount( UInt32 * count ) APPLE_KEXT_OVERRIDE;
    UInt32 getEffectiveCapacity(uint);
    virtual bool checkForWork() APPLE_KEXT_OVERRIDE;

    static UInt32 defaultQueryFreeSpaceHandler(OSObject *,IOSkywalkTxSubmissionQueue*,uint *);
    UInt32 defaultQueryFreeSpace(uint *);

    virtual IOReturn performCommand( UInt32 command, void * data, size_t dataSize ) APPLE_KEXT_OVERRIDE;
    virtual void packetCompletion( IOSkywalkPacket * packet, IOSkywalkPacketQueue * queue, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    
    static IOSkywalkTxSubmissionQueue * withPoolAndServiceClass(IOSkywalkPacketBufferPool *,uint,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,uint *),uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint);
    static IOSkywalkTxSubmissionQueue * withPool(IOSkywalkPacketBufferPool *,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,uint *),uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint);
    static IOSkywalkTxSubmissionQueue * withPool(IOSkywalkPacketBufferPool *,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint);
    virtual bool initWithPool(IOSkywalkPacketBufferPool *,uint,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,uint *),uint (*)(OSObject *,IOSkywalkTxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint);
    virtual void free() APPLE_KEXT_OVERRIDE;
};

#endif
