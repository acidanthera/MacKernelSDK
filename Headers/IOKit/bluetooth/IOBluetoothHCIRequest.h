/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
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

#ifndef __IOKIT_IOBLUETOOTHHCIREQUEST_H
#define __IOKIT_IOBLUETOOTHHCIREQUEST_H

#import <IOKit/system.h>
#include <IOKit/IOService.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/bluetooth/IOBluetoothHostController.h>

// Forward declaration to avoid the need to include IOBluetoothHCIUserLibShared.h
typedef struct BluetoothHCINotificationMessage;

enum
{
    kMaxHCIBufferLength = 512
};

typedef UInt8 BluetoothHCIRequestState;

/*! @enum        BluetoothHCIRequestStates
     @abstract    States of Bluetooth HCI Requests
     @constant    kHCIRequestStateIdle              Doing nothing - neither waiting nor busy. usually waiting for deletion.
     @constant    kHCIRequestStateWaiting        On the wait queue - request has not been processed in any way.
     @constant    kHCIRequestStateBusy            On the busy queue - request is sent and is currently processing
*/

enum BluetoothHCIRequestStates
{
    kHCIRequestStateIdle        = 0,
    kHCIRequestStateWaiting     = 1,
    kHCIRequestStateBusy        = 2
};

class IOBluetoothHCIRequest : public OSObject
{
    OSDeclareDefaultStructors(IOBluetoothHCIRequest)

    friend class IOBluetoothHostController;
    
public:
    static IOBluetoothHCIRequest *      Create( IOCommandGate * commandGate, IOBluetoothHostController * hostController, bool async = true, UInt32 timeout = 5, UInt32 controlFlags = 0 );
    static IOReturn                     Dispose( IOBluetoothHCIRequest * inObject );
    IOReturn                            DisposeRequest();
    
    bool                                init( IOCommandGate * commandGate, IOBluetoothHostController * hostController );
    void                                InitializeRequest();
    virtual void                        free() APPLE_KEXT_OVERRIDE;
    virtual void                        retain() const APPLE_KEXT_OVERRIDE;
    void                                RetainRequest( char * name );
    virtual void                        release() const APPLE_KEXT_OVERRIDE;
    void                                ReleaseRequest( char * name );
    
    const char *                        RequestDescription( const char * name );
    
    IOReturn                            Start();    // Called when a request is started on a transport
    void                                Complete(); // Called when a request is completed on a transport
    
    void                                SetState( BluetoothHCIRequestState inState );
    
    void                                SetCallbackInfo( BluetoothHCIRequestCallbackInfo * inInfo );
    void                                SetCallbackInfoToZero();
    
    void                                SetResultsDataSize( IOByteCount inCount );
    void                                SetResultsBufferPtrAndSize( UInt8 * resultsBuffer, IOByteCount inSize );
    void                                CopyDataIntoResultsPtr( UInt8 * inDataPtr, IOByteCount inSize );
    UInt8 *                             GetResultsBuffer();
    IOByteCount                         GetResultsBufferSize();
    
    BluetoothHCICommandOpCode           GetCommandOpCode();
    
    void                                StartTimer();
    
    static void                         timerFired( OSObject * owner, IOTimerEventSource * sender );
    void                                handleTimeout();
    
    UInt32                              GetCurrentTime();
    void                                SetStartTimeForDelete();
    
    bool                                CompareDeviceAddress( const BluetoothDeviceAddress * inDeviceAddress );
    
protected:
    UInt8                               mPrivateResultsBuffer[kMaxHCIBufferLength * 4];     // Just in case they didn't give a results ptr. 12
    IOByteCount                         mPrivateResultsSize;                                // Result size. 2064
    BluetoothHCITransportID             mTransportID;                                       // Transport ID to use for this request. 2072
    BluetoothHCIRequestState            mState;                                             // Busy, waiting, idle. 2076
    bool                                mAsyncNotify; //2077
    task_t                              mOwningTaskID; //2080
    BluetoothHCIRequestCallbackInfo     mCallbackInfo;                                      // When this request is complete, call this. 2088
    BluetoothHCICommandOpCode           mOpCode; //2128
    BluetoothDeviceAddress              mDeviceAddress; //2130
    BluetoothConnectionHandle           mConnectionHandle; //2136
    BluetoothHCINotificationMessage *   mNotificationMessage; //2144
    IOByteCount                         mNotificationMessageSize; //2152
    
public:
    IOBluetoothHCIRequest *             mNextBusy;                                  // Points to next request element on busy queue. 2160
    IOBluetoothHCIRequest *             mNextWait;                                  // Points to next request element on wait queue. 2168
    IOBluetoothHCIRequest *             mNextAllocated;                             // Points to next allocated request element. 2176
    IOBluetoothHCIRequest *             mPreviousAllocated;                         // Points to next allocated request element. 2184
    BluetoothHCIRequestID               mID;                                        // For internal identification. 2192
    UInt8                               mCommandBuffer[kMaxHCIBufferLength];        // Built-up HCI Command to send to the transport. 2196
    IOByteCount                         mCommandBufferSize;                         // Size of command buffer. 2712
    
    UInt8 *                             mResultsPtr;                                // Result ptr, provided by object creator. 2720
    IOByteCount                         mResultsSize;                               // Result size. 2728
    
    IOCommandGate *                     mCommandGate; //2736
    IOTimerEventSource *                mTimer; //2744
    IOBluetoothHostController *         mHostController; //2752
    
    IOReturn                            mStatus;                                    // Success/failure code of request. 2760
    UInt32                              mTimeout;                                   // Timeout for request to complete, in milliseconds. 2764
    UInt32                              mOriginalTimeout; //2768
    UInt32                              mControlFlags; //2772
    int                                 mPID;                                       // Creating Task 2776
    bool                                mHCIRequestDeleteWasCalled;                 // Fixed rdar://problem/7044168 2780
    UInt32                              mStartTimeForDelete; //2784
    
    // Require and Receive Events
    
    UInt8                               __reserved1; //2788
    
    UInt32                              mExpectedEvent; //2792
    UInt8                               mNumberOfExpectedExplicitCompleteEvents; //2796
    UInt32                              __reserved4; //2800
    
    UInt64                              __reserved5; //2808
    UInt32                              __reserved6; //2816
    UInt32                              mReceivedEvent; //2820
    UInt8                               mExplictCompleteEvent; //2824
    UInt8                               mNumberOfPossibleIntermediateEvents; //2828
    UInt8                               mPossibleIntermediateEvents; //2832
    UInt64                              __reserved10; //2840
    UInt32                              __reserved11; //2848
    UInt32                              mReceivedIntermediateEvents; //2852
    UInt64                              __reserved13; //2864
};

#endif
