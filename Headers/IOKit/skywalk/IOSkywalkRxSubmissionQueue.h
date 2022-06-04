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

#ifndef _IOSKYWALKRXSUBMISSIONQUEUE_H
#define _IOSKYWALKRXSUBMISSIONQUEUE_H

#include <IOKit/skywalk/IOSkywalkPacketQueue.h>

class IOSkywalkRxSubmissionQueue : public IOSkywalkPacketQueue
{
    OSDeclareDefaultStructors( IOSkywalkRxSubmissionQueue )

public:
    
    withPool(IOSkywalkPacketBufferPool *,uint,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkRxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint)
    initWithPool(IOSkywalkPacketBufferPool *,uint,uint,uint,OSObject *,uint (*)(OSObject *,IOSkywalkRxSubmissionQueue*,IOSkywalkPacket * const*,uint,void *),void *,uint)
    retryDequeue(void)
    free(void)
    adjustPacketCounters(void)
    getEffectiveCapacity(uint)
    enable(void)
    disable(void)
    checkForWork(void)
    gatedDequeue(void *)
    requestDequeue(void *,uint)
    packetCompletion(IOSkywalkPacket *,IOSkywalkPacketQueue *,uint)
    getPacketCount(void)
    initialize(void *)
    finalize(void)
    performCommand(uint,void *,ulong)
    addReporters(IOService *,OSSet *)
    getReportChannelValue(ulong long)
    
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  0 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  1 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  2 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  3 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  4 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  5 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  6 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  7 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  8 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue,  9 );
    OSMetaClassDeclareReservedUnused( IOSkywalkRxSubmissionQueue, 10 );

protected:
    
};

#endif
