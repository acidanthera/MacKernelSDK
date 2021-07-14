/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
 * Copyright (c) 2021 cjiang. All rights reserved.
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
 
#ifndef _IOBLUETOOTH_HOSTCONTROLLER_UARTTRANSPORT_H
#define _IOBLUETOOTH_HOSTCONTROLLER_UARTTRANSPORT_H

#include <IOKit/bluetooth/transport/IOBluetoothHostControllerTransport.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/bluetooth/IOBluetoothSerialClient.h>

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
