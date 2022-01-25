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

#ifndef _IO80211FLOWQUEUE_H
#define _IO80211FLOWQUEUE_H

#include <IOKit/network/IOEthernetController.h>
#include <IOKit/80211/apple80211_var.h>

struct IO80211FlowQueueHash
{
    UInt8      ac;
    UInt8      interface;
    ether_addr address;
} __attribute__((packed));

class IO80211FlowQueue : public OSObject
{
    OSDeclareDefaultStructors( IO80211FlowQueue )

public:
    virtual UInt32 enqueuePacket( mbuf_t packet );
    virtual UInt32 queueSpace();
    virtual UInt32 queueSize();
    virtual UInt32 pendingPackets();
    virtual void pause();
    virtual void unPause();
    virtual bool isPaused();
    virtual UInt32 DEBUG_totalStagedPackets();
    virtual UInt32 DEBUG_curStagedPackets();
    virtual UInt32 print( void * userPrintCtx );

protected:
    IO80211FlowQueueHash _hash; // 16
    void * _buf;                // 24
    bool _paused;               // 32
};

class IO80211FlowQueueLegacy : public IO80211FlowQueue
{
    OSDeclareDefaultStructors( IO80211FlowQueueLegacy )
    
public:
    IO80211FlowQueueLegacy * withParamaters( const ether_addr & address, UInt8 ac, UInt8 interface );
    bool initWithParamaters( const ether_addr & address, UInt8 ac, UInt8 interface );
    
    virtual UInt32 enqueuePacket( mbuf_t packet ) APPLE_KEXT_OVERRIDE;
    
    virtual UInt32 queueSpace() APPLE_KEXT_OVERRIDE;
    virtual UInt32 queueSize() APPLE_KEXT_OVERRIDE;
    
    virtual bool isPaused() APPLE_KEXT_OVERRIDE;
};

#endif
