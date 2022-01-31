/*
 * Released under "The BSD 3-Clause License"
 *
 * Copyright (c) 2021 cjiang. All rights reserved.
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

#ifndef _IOSKYWALKPACKETBUFFERPOOL_H
#define _IOSKYWALKPACKETBUFFERPOOL_H

#include <IOKit/skywalk/IOSkywalkPacketBuffer.h>

extern void pp_ctx_retain( void * target );
extern void pp_ctx_release( void * target );

struct IOSkywalkPacketDescriptor;
struct IOSkywalkPacketBufferDescriptor;
struct IOSkywalkMemorySegmentDescriptor;

class IOSkywalkPacketBufferPool : public OSObject
{
    OSDeclareDefaultStructors( IOSkywalkPacketBufferPool )

    struct PoolOptions
    {
        uint32_t packetCount; // 0
        uint32_t bufferCount; // 4
        uint32_t bufferSize; // 8
        uint32_t maxBuffersPerPacket; // 12
        uint32_t memorySegmentSize; // 16
        uint32_t poolFlags; // 20
        // 24
    };
    
public:
    static IOSkywalkPacketBufferPool * withName( const char * name, OSObject * owner, UInt32 flags, const PoolOptions * options );
    virtual bool initWithName( const char * name, void * owner, UInt32 flags, const PoolOptions * options );
    virtual bool initWithName( const char * name, OSObject * owner, UInt32 flags, const PoolOptions * options );
    virtual void free() APPLE_KEXT_OVERRIDE;
    void disposeAllPackets();
    void disposeAllBuffers();
    virtual void release() const APPLE_KEXT_OVERRIDE;
    void releaseAllPackets();
    void releaseAllMemorySegments();
    
    void reportingChangeNotification();
    
    void segmentConstructor( struct kern_pbufpool * pbufPool, struct sksegment * segment, IOMemoryDescriptor * md );
    void segmentDestructor( struct kern_pbufpool * pbufPool, struct sksegment * segment, IOMemoryDescriptor * md );
    bool createSegmentBuffers( IOSkywalkMemorySegment * segment, IOOptionBits options, bool subDesc );
    void destroySegmentBuffers( IOSkywalkMemorySegment * segment );
    IOReturn prepareMemorySegment( IOSkywalkMemorySegment * segment, IOBufferMemoryDescriptor * buffer, UInt64 offset );
    IOSkywalkMemorySegment * getMemorySegmentWithHandle( struct sksegment * handle );
    IOSkywalkPacket * getPacketWithHandle( UInt64 handle );
    IOSkywalkPacket * getPacketWithIndex( UInt32 index );
    IOSkywalkPacketBuffer * getPacketBufferWithBufletHandle( kern_buflet_t handle );
    IOSkywalkPacketBuffer * getPacketBufferWithSegmentInfo( sksegment * segment, UInt32 );
    const char * getPoolName();
    
    void checkInPacketQueue( const IOSkywalkPacketQueue * queue );
    void checkOutPacketQueue( const IOSkywalkPacketQueue * queue );
    void checkInReportingService( IOService * service, const IOSkywalkInterface * interface );
    void checkOutReportingService( const IOService * service );
    void createReportersForService( IOService * service, const IOSkywalkInterface * interface );
    void removeReportersForService( const IOService * service );
    void addPoolReporter( IOService * service, OSSet * reportSet );
    OSSet * copyReportersForService( const IOService * service );
    UInt64 getReportChannelValue( UInt64 reportChannel );
    
    virtual IOReturn allocatePacket( uint,IOSkywalkPacket **,uint );
    virtual IOReturn allocatePackets( uint,uint *,IOSkywalkPacket **,uint );
    virtual IOReturn deallocatePacket( IOSkywalkPacket * packet );
    virtual IOReturn deallocatePackets( IOSkywalkPacket ** packets, uint32_t packetsCount );
    virtual IOReturn deallocatePacketChain( UInt64 );
    virtual IOReturn allocatePacketBuffer( IOSkywalkPacketBuffer **,uint );
    virtual IOReturn allocatePacketBuffers( uint *,IOSkywalkPacketBuffer **,uint );
    virtual IOReturn deallocatePacketBuffer( IOSkywalkPacketBuffer * buffer );
    virtual IOReturn deallocatePacketBuffers( IOSkywalkPacketBuffer ** buffers, uint32_t buffersCount );
    
    virtual IOReturn newPacket( IOSkywalkPacketDescriptor * desc, IOSkywalkPacket ** outPacket );
    virtual IOReturn newPacketBuffer( IOSkywalkPacketBufferDescriptor * desc, IOSkywalkPacketBuffer ** outBuffer );
    virtual IOReturn newMemorySegment( IOSkywalkMemorySegmentDescriptor * desc, IOSkywalkMemorySegment ** outSegment );
    
protected:
    void * _reserved; // 16
    struct kern_pbufpool * mPbufPool; // 24
    OSObject * mProvider; // 32
    OSString * mPoolName; // 144
    thread_call_t mReportingChangeNotificationThread; // 168
    // 176
};

// 184

#endif
