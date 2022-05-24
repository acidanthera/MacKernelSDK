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

#ifndef _IO80211FLOWQUEUEDATBASE_H
#define _IO80211FLOWQUEUEDATBASE_H

#include <IOKit/80211/IO80211FlowQueue.h>

typedef bool (*IO80211FlowQueueResultAction)( IO80211FlowQueue * queue, void * arg );
typedef void (*IO80211FlowQueueAction)( IO80211FlowQueue * queue, void * arg );

class IO80211FlowQueueDatabase : public OSObject
{
    OSDeclareDefaultStructors( IO80211FlowQueueDatabase )

public:
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    void lockDatabase();
    void unlockDatabase();
    
    IOReturn insert( IO80211FlowQueue * que );
    void remove( UInt64 hash );
    
    IO80211FlowQueue * map( IO80211FlowQueueResultAction action, void * arg );
    IO80211FlowQueue * find( IO80211FlowQueueResultAction action, void * arg );
    IO80211FlowQueue * find( UInt64 hash );
    void flush( IO80211FlowQueueAction action, void * arg );
    
    UInt32 pendingPackets( UInt8 interface );
    UInt32 packetSpace( UInt8 interface );
    UInt32 queueSize( UInt8 interface );
    
    UInt32 print( void * userPrintCtx );
    
protected:
    /*! @var _dataBase
     *   The data base in which all flow queues of this object are stored. */
    
    IO80211FlowQueue * _dataBase[200]; // 16
    
    /*! @var _foundQueue
     *   The result request found in find() will be stored in this member variable so as to accelerate the next find request. */
    
    IO80211FlowQueue * _foundQueue; // 1616
    SInt32 _queueSize; // 1624
    IOSimpleLock * _dataBaseLock; // 1632
};

#endif
