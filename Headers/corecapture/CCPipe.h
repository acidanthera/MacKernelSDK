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

#ifndef _CCPIPE_H
#define _CCPIPE_H

#include <corecapture/CCCapture.h>

struct CCPipeOptions
{
    UInt32 pipeType; // 0
    UInt32 logType;  // 4
    UInt32 logDataType; // 8
    UInt32 logPolicy; // 12
    UInt64 pipeSize; // 16
    UInt64 minLogSizeToNotify; // 24
    UInt32 notifyThresold; // 32
    char fileName[0x100]; // 36
    char logIdentifier[0x100]; // 292
    UInt32 logSize; // 548
    UInt32 numFiles; // 552
    // 560, q, 216
    // 568, q, 224
    UInt32 fileOptions; // 576
    char directoryName[0x100]; // 580
    // 836, d, 256
    // 840, d, 260
    // 844, d, 264
};

class IOSimpleReporter;

class CCPipe : public IOService
{
    OSDeclareAbstractStructors( CCPipe )
    
public:
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual void detach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn configureReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport( IOReportChannelList * channels, IOReportUpdateAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    bool createReportSet();
    IOReturn createReportersAndLegend( const char * pipeName );
    void destroyReporters();
    
    void publishStatistics();
    void updateStatistics( bool );
    
    virtual bool startPipe();
    virtual void stopPipe();
    virtual UInt32 generateStreamId();
    
    virtual void clientClose() = 0;
    virtual bool isClientConnected() = 0;
    
    virtual void removeCapture();
    virtual void profileLoaded();
    virtual void profileRemoved();
    
    static void ensureValidTimestamp( CCTimestamp * timestamp );
    
    virtual IOReturn capture( CCTimestamp * timestamp, const char * str ) = 0;
    virtual IOReturn capture( CCTimestamp timestamp, const char * str ) = 0;
    
    void capturePipesUnder( IOService * owner, CCTimestamp timestamp, const char * str );
    void capturePipesWithOwner( const char * ownerName, CCTimestamp timestamp, const char * str );
    
    virtual void updateAllReports();
    CCPipe * withOwnerNameCapacity( IOService * owner, const char * ownerName, const char * pipeName, const CCPipeOptions * options );
    virtual bool initWithOwnerNameCapacity( IOService * owner, const char * ownerName, const char * pipeName, const CCPipeOptions * options );
    virtual void addCapture();
    
protected:
    static const IORegistryPlane * gCoreCapturePlane;
    static const IORegistryEntry * kCoreCapturePlane;
    
    IOSimpleReporter * mPipeReporter; // 136
    IOService * mOwner; // 144
    UInt32 mStreamID; // 152
    bool pipeStarted; // 156
    // 160
    
    CCCapture * mCaptureNub; // 248
    // 256
    
    OSSet * mReporterSet; // 328
};

// 336

#endif
