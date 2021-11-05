/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
 * Copyright (c) 2021 cjiang.
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

#ifndef _IOKIT_IOBLUETOOTHHOSTCONTROLLERUSBTRANSPORT_H
#define _IOKIT_IOBLUETOOTHHOSTCONTROLLERUSBTRANSPORT_H

#include <IOKit/bluetooth/transport/IOBluetoothHostControllerTransport.h>
#include <IOKit/usb/IOUSBHostDevice.h>
#include <IOKit/usb/IOUSBHostInterface.h>

extern const IORegistryPlane * gIODTPlane;
extern const IORegistryPlane * gIOServicePlane;
extern const OSSymbol        * gIOGeneralInterest;

class IOBluetoothMemoryDescriptorRetainer;

class IOBluetoothHostControllerUSBTransport : public IOBluetoothHostControllerTransport
{
    OSDeclareAbstractStructors(IOBluetoothHostControllerUSBTransport)

    friend class IOBluetoothHostController;

public:
    virtual bool init( OSDictionary * dictionary = NULL ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual IOService * probe( IOService * provider, SInt32 * score ) APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool terminateWL( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    virtual bool InitializeTransportWL( IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn SetRemoteWakeUp( bool ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn DoDeviceReset( UInt16 ) APPLE_KEXT_OVERRIDE;
    
    virtual void AbortPipesAndClose(bool stop, bool release) APPLE_KEXT_OVERRIDE;
    virtual bool HostSupportsSleepOnUSB() APPLE_KEXT_OVERRIDE;
    
    virtual bool ConfigurePM( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long maxCapabilityForDomainState( IOPMPowerFlags domainState ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long initialPowerStateForDomainState( IOPMPowerFlags domainState ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setPowerStateWL( unsigned long powerStateOrdinal, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn RequestTransportPowerStateChange( IOBluetoothHCIControllerInternalPowerState powerState, char * calledByFunction ) APPLE_KEXT_OVERRIDE;
    virtual void CompletePowerStateChange( char * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn ProcessPowerStateChangeAfterResumed( char * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn powerStateWillChangeTo( IOPMPowerFlags capabilities, unsigned long stateNumber, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn powerStateWillChangeToWL( IOOptionBits options, void * ) APPLE_KEXT_OVERRIDE;
    virtual void systemWillShutdownWL( IOOptionBits options, void * ) APPLE_KEXT_OVERRIDE;
    
    virtual bool ControllerSupportWoBT() APPLE_KEXT_OVERRIDE;
    virtual UInt16 GetControllerVendorID() APPLE_KEXT_OVERRIDE;
    virtual UInt16 GetControllerProductID() APPLE_KEXT_OVERRIDE;
    virtual BluetoothHCIPowerState GetRadioPowerState() APPLE_KEXT_OVERRIDE;
    virtual void SetRadioPowerState( BluetoothHCIPowerState ) APPLE_KEXT_OVERRIDE;
    virtual void ResetBluetoothDevice() APPLE_KEXT_OVERRIDE;
    virtual void GetInfo(void * outInfo) APPLE_KEXT_OVERRIDE;
    static IOReturn SetIdlePolicyValueAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual IOReturn SetIdlePolicyValue(uint32_t idleTimeoutMs) APPLE_KEXT_OVERRIDE;
    virtual bool TransportWillReEnumerate() APPLE_KEXT_OVERRIDE;
    
    static IOReturn MessageReceiver(void * target, void * refCon, UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize);
    virtual IOReturn HandleMessage(UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize);
    static IOReturn HandleMessageAction(OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5);
    
    virtual IOReturn SendHCIRequest(UInt8 * buffer, IOByteCount size) APPLE_KEXT_OVERRIDE;
    static void DeviceRequestCompleteHandler(void * owner, void * parameter, IOReturn status, uint32_t bytesTransferred);
    static IOReturn DeviceRequestCompleteAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
    
    virtual void UpdateSCOConnections(UInt8, UInt32) APPLE_KEXT_OVERRIDE;
    virtual void SetupTransportSCOParameters() APPLE_KEXT_OVERRIDE;
    virtual void DestroyTransportSCOParameters() APPLE_KEXT_OVERRIDE;
    
    virtual bool ConfigureDevice();
    virtual UInt8 GetInterfaceNumber(IOUSBHostInterface * interface);
    virtual IOUSBHostInterface * FindNextInterface(IOUSBHostInterface *, UInt16, UInt16, UInt16, UInt16);
    virtual IOUSBHostPipe * FindNextPipe(IOUSBHostInterface *, UInt8, UInt8, Descriptor *);
    virtual bool FindInterfaces();
    
    virtual bool StartLMPLogging() APPLE_KEXT_OVERRIDE;
    virtual bool StartLMPLoggingBulkPipeRead() APPLE_KEXT_OVERRIDE;
    virtual IOReturn ToggleLMPLogging( UInt8 * ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn TransportLMPLoggingBulkOutWrite( UInt8, UInt8 ) APPLE_KEXT_OVERRIDE;
    
    virtual bool StartInterruptPipeRead() APPLE_KEXT_OVERRIDE;
    virtual bool StopInterruptPipeRead() APPLE_KEXT_OVERRIDE;
    static void InterruptReadHandler(void * owner, void * parameter, IOReturn status, uint32_t bytesTransferred);
    
    virtual bool StartBulkPipeRead() APPLE_KEXT_OVERRIDE;
    virtual bool StopBulkPipeRead() APPLE_KEXT_OVERRIDE;
    static void BulkInReadHandler(void * owner, void * parameter, IOReturn status, uint32_t bytesTransferred);
    
    virtual bool StartIsochPipeRead();
    virtual bool StopIsochPipeRead();
    static void IsochInReadHandler(void * owner, void * parameter, IOReturn status, IOUSBHostIsochronousFrame * frameList);
    virtual void ResetIsocFrames(IOUSBHostIsochronousFrame * isocFrames, UInt32 numberOfFrames);
    
    virtual bool StopAllPipes();
    virtual bool StartAllPipes();
    virtual void WaitForAllIOsToBeAborted();
    virtual void ReceiveInterruptData(void * data, UInt32 dataSize, bool);
    
    virtual IOReturn TransportBulkOutWrite(void * buffer) APPLE_KEXT_OVERRIDE;
    virtual IOReturn BulkOutWrite(IOMemoryDescriptor *);
    static void BulkOutWriteCompleteHandler(void * owner, void * parameter, IOReturn status, uint32_t bytesTransferred);
    static IOReturn BulkOutWriteCompleteAction(OSObject * onwer, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5,void * arg6);
    virtual void HandleBulkOutWriteTimeout(IOBluetoothMemoryDescriptorRetainer *);
    static void BulkOutWriteTimerFired(OSObject * target, IOTimerEventSource * sender);
    
    virtual void HandleIsochData(void *, int, IOUSBHostIsochronousFrame *);
    virtual IOReturn TransportIsochOutWrite(void * memDescriptor, void *, IOOptionBits) APPLE_KEXT_OVERRIDE;
    virtual IOReturn IsochOutWrite(IOMemoryDescriptor * memDescriptor, IOBluetoothSCOMemoryDescriptorRetainer *, int);
    static void IsochOutWriteCompleteHandler(void * owner, void * parameter, IOReturn status, IOUSBHostIsochronousFrame * frameList);
    
    virtual void LogData(void * data, UInt64, IOByteCount size);
    virtual bool USBControllerSupportsSuspend();
    
    virtual bool SystemGoingToSleep();
    virtual IOReturn PrepareControllerForSleep();
    virtual bool PrepareControllerWakeFromSleep();
    virtual bool PrepareControllerForPowerOff(bool);
    virtual bool PrepareControllerForPowerOn();
    virtual bool SystemWakeCausedByBluetooth();
    
    virtual IOReturn ProcessG3StandByWake();
    virtual IOReturn ReConfigure();
    virtual bool NeedToTurnOnUSBDebug();
    
    virtual IOReturn HardReset() APPLE_KEXT_OVERRIDE;
    
private:
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 0);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 1);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 2);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 3);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 4);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 5);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 6);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 7);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 8);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 9);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 10);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 11);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 12);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 13);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 14);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 15);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 16);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 17);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 18);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 19);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 20);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 21);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 22);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerUSBTransport, 23);

protected:
    IOUSBHostDevice * mBluetoothUSBHostDevice; //328
    IOUSBHostDevice * mBluetoothUSBHub; //336
    UInt16 mActiveDeviceRequestIOCount; //344
    UInt16 mVendorID; //346
    UInt16 mProductID; //348
    IOUSBHostInterface * mInterface; //352
    IOUSBHostInterface * mIsochInterface; //360
    UInt8 mInterfaceNumber; //368
    UInt8 mIsochInterfaceNumber; //369
    
    uint8_t mEmptyInterruptReadData[1021]; //370, an empty buffer used to determine if the buffer stored in mInterruptReadDataBuffer is empty
    IOUSBHostPipe * mInterruptPipe; //1392
    Descriptor * mInterruptDescriptor; //1400
    IOBufferMemoryDescriptor * mInterruptReadDataBuffer; //1408
    IOUSBHostCompletion mInterruptCompletion; //1416 (size = 24)
    bool mInterruptPipeStarted; //1440
    UInt16 mInterruptPipeOutstandingIOCount; //1442
    
    IOUSBHostPipe * mBulkInPipe; //1448
    Descriptor * mBulkInDescriptor; //1456
    IOBufferMemoryDescriptor * mBulkInReadDataBuffer; //1464
    IOUSBHostCompletion mBulkInCompletion; //1472, 24
    bool mBulkInPipeStarted; //1496
    UInt16 mBulkInPipeOutstandingIOCount; //1498
    
    IOUSBHostPipe * mBulkOutPipe; //1504
    IOUSBHostCompletion mBulkOutCompletion; //1512 + 24
    
    IOUSBHostPipe * mIsochInPipe; //1536
    IOBufferMemoryDescriptor ** mIsochInReadDataBufferList; //1544, contains 2 IOBufferMemoryDescriptor
    int mIsochInReadDataBufferStates[2]; //1552
    IOUSBHostIsochronousCompletion * mIsochInCompletionRoutineList; //1560
    UInt32 mIsochInPipeNumReads; //1568
    bool mIsochInReadsComplete; //1572
    IOUSBHostIsochronousFrame * mIsochInFrames; //1576
    UInt64 mIsochInFrameNumber; //1584
    vm_size_t mIsochInReadDataBufferLength; //1592
    UInt32 mIsochInPipeNumFrames; //1596
    UInt32 mIsochInFrameNumRequestedBytes; //1600, The number of bytes requested to transfer for this frame
    
    UInt32 mIsochOutFrameNumRequestedBytes; //1604
    IOUSBHostPipe * mIsochOutPipe; //1608
    IOUSBHostIsochronousCompletion mIsochOutCompletion; //1616 + 24
    IOUSBHostIsochronousFrame * mIsochOutFrames; //1640
    UInt32 mIsochOutPipeNumFrames; //1648
    UInt64 mIsochOutFrameNumber; //1656

    UInt32 mPreviousIsochOutPipeNumFrames; //1664
    UInt8 mInterruptReadNumRetries; //1668
    UInt8 mBulkInReadNumRetries; //1669
    IONotifier * mMessageReceiverNotifier; //1672
    UInt64 mPreviousIsochOutFrameNumber; //1680

    UInt8 * mInterruptData; //1688
    UInt8 mInterruptDataLength; //1696
    UInt32 mInterruptDataPosition; //1700
    UInt16 mStopInterruptPipeReadCounter; //1704

    bool mInterfaceFound; //1706
    bool mIsochInterfaceFound; //1707
    bool yy; //1708
    uint8_t __reserved1; //1709
    bool isServiceRegistered; //1710
    bool xx; //1711
    UInt8 xxx; //1712
    bool __reserved5; //1713
    bool mMatchedOnInterface; //1714
    UInt32 mInterruptSleepMs; //1716
    bool mAbortPipesAndCloseCalled; //1720
    bool u; //1721
    bool mIOClassIsAppleUSBXHCIPCI; //1722
    bool mPipesStarted; //1723
    bool v; //1724 pm state
    bool n; //1725 pm state
    bool mHostDeviceStarted; //1726
    bool mHubStarted; //1727

    struct ExpansionData
    {
        void * mRefCon;
    };
    ExpansionData * mExpansionData; //1728
};

#endif
