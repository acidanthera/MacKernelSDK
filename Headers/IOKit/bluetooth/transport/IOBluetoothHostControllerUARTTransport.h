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
 
#ifndef _IOKIT_IOBLUETOOTHHOSTCONTROLLERUARTTRANSPORT_H
#define _IOKIT_IOBLUETOOTHHOSTCONTROLLERUARTTRANSPORT_H

#include <IOKit/bluetooth/transport/IOBluetoothHostControllerTransport.h>
#include <IOKit/bluetooth/serial/IOBluetoothSerialClient.h>
#include <IOKit/IOInterruptEventSource.h>

#ifndef __MAC_OS_X_VERSION_MIN_REQUIRED
#error "Missing macOS target version"
#endif

extern void TimeOutHandler( OSObject * owner, IOTimerEventSource * sender );

struct UARTPacket // guessed name... will try to refine it with the UART protocol
{
    UInt8 type;
    UInt8 data[65540];
};

struct ACLDataPacketHeader
{
    UInt16 offset; //0
    UInt16 dataSize; //2
};

struct SCODataPacketHeader
{
    UInt16 offset; //0
    UInt8 dataSize; //2
};

struct EventDataPacketHeader
{
    UInt8 offset; //0
    UInt8 dataSize; //1
};

class IOBluetoothHostControllerUARTTransport : public IOBluetoothHostControllerTransport
{
    OSDeclareAbstractStructors(IOBluetoothHostControllerUARTTransport)
    
public:
    virtual bool init( OSDictionary * dictionary = NULL ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual IOService * probe( IOService * provider, SInt32 * score ) APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool terminateWL( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    
    virtual bool ConfigurePM( IOService * policyMaker ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setPowerStateWL( unsigned long powerStateOrdinal, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    virtual void CompletePowerStateChange( char * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn powerStateWillChangeToWL( IOOptionBits options, void * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn RequestTransportPowerStateChange( IOBluetoothHCIControllerInternalPowerState powerState, char * ) APPLE_KEXT_OVERRIDE;
    virtual void systemWillShutdownWL( IOOptionBits options, void * ) APPLE_KEXT_OVERRIDE;
    
    virtual UInt16 GetControllerVendorID() APPLE_KEXT_OVERRIDE;
    virtual UInt16 GetControllerProductID() APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn SendHCIRequest( UInt8 *, UInt64 ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn TransportBulkOutWrite( void * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn TransportIsochOutWrite( void *, void *, IOOptionBits ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn TransportSendSCOData( void * ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn SendUART(UInt8 * buffer, UInt32 size);
    static IOReturn StaticProcessACLSCOEventData(void *, int);
    virtual IOReturn ProcessACLSCOEventData();
    virtual void GetInfo(void * outInfo) APPLE_KEXT_OVERRIDE;
    virtual IOReturn AcquirePort(bool);
    virtual IOReturn ReleasePort();
    virtual IOReturn WaitForReceiveToBeReady(bool);
    virtual void NewSCOConnection() APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn SetLMPLogging();
    virtual bool StartLMPLogging() APPLE_KEXT_OVERRIDE;
    virtual bool StopLMPLogging();
    virtual IOReturn ToggleLMPLogging( UInt8 * ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn DoDeviceReset( UInt16 ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn DequeueDataInterruptEventGated(IOInterruptEventSource * sender, int count);
    
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_VERSION_11_0
    virtual void DumpTransportProviderState() APPLE_KEXT_OVERRIDE;
#endif
    
protected:
    UARTPacket * mPacket; //328
    UInt8 * mDataPacket; //336
    IORS232SerialStreamSync * mProvider; //344
    //352
    //360
    //364
    //368
    bool mSkipBluetoothFirmwareBoot; //369
    IOWorkLoop * mUARTTransportWorkLoop; //376
    IOInterruptEventSource::Action mDequeueDataInterruptAction; //384
    IOInterruptEventSource * mDequeueDataInterrupt; //392
    IOWorkLoop * mUARTTransportTimerWorkLoop; //400
    IOCommandGate * mUARTTransportTimerCommandGate; //408
    IOTimerEventSource * mUARTTransportTimer; //416
    bool mUARTTransportTimerHasTimeout; //424
    UInt32 mSlowEnqueueData; //428
    UInt32 mLongestEnqueueDataCallMicroseconds; //432
};

#endif
