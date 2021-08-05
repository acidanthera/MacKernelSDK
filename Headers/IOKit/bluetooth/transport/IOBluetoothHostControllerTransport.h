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
/*!
 *   @header
 *   This header contains the definition of the IOBluetoothHostControllerTransport class, a subclass of IOService that serves as the base class for all the various transports in the IOBluetoothFamily.
 *
 */

#ifndef _IOBLUETOOTH_HOSTCONTROLLER_TRANSPORT_H
#define _IOBLUETOOTH_HOSTCONTROLLER_TRANSPORT_H

#include <Availability.h>
#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/bluetooth/Bluetooth.h>
#include <IOKit/bluetooth/IOBluetoothHostController.h>
#include <IOKit/bluetooth/IOBluetoothHCIController.h>
#include <IOKit/bluetooth/IOBluetoothACPIMethods.h>

#ifndef __MAC_OS_X_VERSION_MIN_REQUIRED
#error "Missing macOS target version"
#endif

extern void BluetoothSleepTimeOutOccurred( OSObject * owner, IOTimerEventSource * sender );

const char * gInternalPowerStateString[7] = { "OFF", "ON", "SLEEP", "IDLE", "OFF", "IDLE", "ON" };
const char * gOrdinalPowerStateString[3] = { "OFF", "IDLE", "ON" };
const char * gPowerManagerSleepTypeString[9] =
{
    "kIOPMSleepTypeInvalid",
    "kIOPMSleepTypeAbortedSleep",
    "kIOPMSleepTypeNormalSleep",
    "kIOPMSleepTypeSafeSleep",
    "kIOPMSleepTypeHibernate",
    "kIOPMSleepTypeStandby",
    "kIOPMSleepTypePowerOff",
    "kIOPMSleepTypeDeepIdle",
    "kIOPMSleepTypeLast"
};
const char * gPowerManagerSleepTypeShortString[9] =
{
    "SleepTypeInvalid",
    "SleepTypeAbortedSleep",
    "SleepTypeNormalSleep",
    "SleepTypeSafeSleep",
    "SleepTypeHibernate",
    "SleepTypeStandby",
    "SleepTypePowerOff",
    "SleepTypeDeepIdle",
    "SleepTypeLast"
};

extern const IORegistryPlane * gIODTPlane;

/*! @class IOBluetoothHostControllerTransport
 *   @abstract The base class for IOBluetoothFamily transports.
 *   @discussion ???
 *
 */

class IOBluetoothHostControllerTransport : public IOService
{
    OSDeclareAbstractStructors(IOBluetoothHostControllerTransport)
    
    friend class IOBluetoothHCIController;
    friend class IOBluetoothHostController;
    
public:
/*! @function init
 *   @abstract Initializes member variables of IOBluetoothHostControllerTransport.
     @discussion This function calls CreateOSLogObject() to create mInternalOSLogObject, sets the initial value for various members of the class, calls GetNVRAMSettingForSwitchBehavior() to set up mSwitchBehavior, and creates the ExpansionData. It calls super::init() at the end to initialize its inherited members.
 *   @result Result of IOService::init(). */
    
    virtual bool init( OSDictionary * dictionary = NULL ) APPLE_KEXT_OVERRIDE;
    
/*! @function free
 *   @abstract Frees data structures that were allocated the class was initialized. */
    
    virtual void free() APPLE_KEXT_OVERRIDE;
    
/*! @function probe
 *   @abstract Probes a matched device to see if it can be used.
 *   @discussion Aside from running probe() of the super class, this function would check the SkipIOBluetoothHostControllerUSBTransport property in the gIODTPlane. If it exists, the function will terminate returning NULL as Bluetooth USB Transport should be skipped.
 *   @param provider The registered IOService object that matches a driver personality's matching dictionary.
 *   @param score The driver's probe score.
 *   @result If SkipIOBluetoothHostControllerUSBTransport does not exist, the result of IOService::probe() would be returned. Otherwise, the result would be NULL. */
    
    virtual IOService * probe( IOService * provider, SInt32 * score ) APPLE_KEXT_OVERRIDE;
    
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    virtual IOCommandGate * getCommandGate() const;
    virtual bool setTransportWorkLoop( void *, IOWorkLoop * inWorkLoop );
    
    virtual bool terminate( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    static IOReturn terminateAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual bool terminateWL( IOOptionBits options = 0 );
    
    virtual bool InitializeTransport();
    static IOReturn InitializeTransportAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual bool InitializeTransportWL( IOService * provider );
    
    virtual OSObject * getPropertyFromTransport( const OSSymbol * aKey );
    virtual OSObject * getPropertyFromTransport( const OSString * aKey );
    virtual OSObject * getPropertyFromTransport( const char * aKey );
    
    virtual IOReturn SetRemoteWakeUp( bool );
    virtual IOReturn DoDeviceReset( UInt16 );
    
    virtual void AbortPipesAndClose( bool, bool );
    virtual bool HostSupportsSleepOnUSB();
    
    virtual bool StartLMPLogging();
    virtual bool StartLMPLoggingBulkPipeRead();
    virtual bool StartInterruptPipeRead();
    virtual bool StopInterruptPipeRead();
    virtual bool StartBulkPipeRead();
    virtual bool StopBulkPipeRead();
    
    virtual IOReturn TransportBulkOutWrite( void * );
    virtual IOReturn TransportIsochOutWrite( void *, void *, IOOptionBits );
    virtual IOReturn TransportSendSCOData( void * );
    virtual IOReturn TransportLMPLoggingBulkOutWrite( UInt8, UInt8 );
    
    virtual IOReturn SendHCIRequest(UInt8 * buffer, IOByteCount size);
    virtual void UpdateSCOConnections( UInt8, UInt32 );
    virtual IOReturn ToggleLMPLogging( UInt8 * );
    
    virtual IOReturn CallConfigPM();
    virtual bool ConfigurePM( IOService * policyMaker );
    
    virtual unsigned long maxCapabilityForDomainState( IOPMPowerFlags domainState ) APPLE_KEXT_OVERRIDE;
    virtual unsigned long initialPowerStateForDomainState( IOPMPowerFlags domainState ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn setPowerState( unsigned long powerStateOrdinal, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    static IOReturn setPowerStateAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual IOReturn setPowerStateWL( unsigned long powerStateOrdinal, IOService * whatDevice );
    
    virtual IOReturn RequestTransportPowerStateChange( IOBluetoothHCIControllerInternalPowerState powerState, char * name );
    virtual IOReturn WaitForControllerPowerState( IOBluetoothHCIControllerInternalPowerState powerState, char * );
    virtual IOReturn WaitForControllerPowerStateWithTimeout( IOBluetoothHCIControllerInternalPowerState powerState, uint32_t timeout, char *, bool );
    virtual void CompletePowerStateChange( char * );
    virtual IOReturn ProcessPowerStateChangeAfterResumed( char * );
    
    virtual IOReturn setAggressiveness( unsigned long type, unsigned long newLevel ) APPLE_KEXT_OVERRIDE;
    static IOReturn setAggressivenessAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual bool setAggressivenessWL( unsigned long type, unsigned long newLevel );
    
    virtual IOReturn powerStateWillChangeTo( IOPMPowerFlags capabilities, unsigned long stateNumber, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    static IOReturn powerStateWillChangeToAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual IOReturn powerStateWillChangeToWL( IOOptionBits options, void * );
    
    virtual void systemWillShutdown( IOOptionBits specifier ) APPLE_KEXT_OVERRIDE;
    static IOReturn systemWillShutdownAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual void systemWillShutdownWL( IOOptionBits options, void * );

    virtual IOBluetoothHCIControllerInternalPowerState GetCurrentPowerState();
    virtual IOBluetoothHCIControllerInternalPowerState GetPendingPowerState();
    
    virtual IOReturn ChangeTransportPowerStateFromIdleToOn( char * name );
    virtual IOReturn ChangeTransportPowerState( unsigned long ordinal, bool willWait, IOBluetoothHCIControllerInternalPowerState powerState, char * name );
    virtual IOReturn WaitForControllerPowerStateChange( IOBluetoothHCIControllerInternalPowerState, char * );
    
    virtual IOReturn WakeupSleepingPowerStateThread();
    virtual bool ControllerSupportWoBT();
    virtual UInt16 GetControllerVendorID();
    virtual UInt16 GetControllerProductID();
    virtual BluetoothHCIPowerState GetRadioPowerState();
    virtual void SetRadioPowerState( BluetoothHCIPowerState powerState );
    virtual bool GetNVRAMSettingForSwitchBehavior();
    virtual UInt32 GetControllerLocationID();
    virtual bool GetBuiltIn();
    virtual bool GetSupportPowerOff();
    virtual bool IsHardwareInitialized();
    virtual UInt32 GetHardwareStatus();
    virtual void ResetHardwareStatus();
    virtual UInt32 ConvertAddressToUInt32(void * address);
    virtual void SetActiveController(bool);
    virtual void ResetBluetoothDevice();
    virtual IOReturn TransportCommandSleep(void *, UInt32, char *, bool);
    virtual void ReadyToGo( bool oneThread );
    virtual bool TerminateCalled();
    virtual void GetInfo( void * outInfo );
    virtual IOReturn CallPowerManagerChangePowerStateTo(unsigned long ordinal, char *);
    virtual UInt16 GetControllerTransportType();
    virtual bool SupportNewIdlePolicy();
    
    virtual void CheckACPIMethodsAvailabilities();
    virtual IOReturn SetBTRS();
    virtual IOReturn SetBTPU();
    virtual IOReturn SetBTPD();
    virtual IOReturn SetBTRB(bool);
    virtual IOReturn SetBTLP(bool);
    
    virtual void NewSCOConnection();
    
    virtual void retain() const APPLE_KEXT_OVERRIDE;
    virtual void release() const APPLE_KEXT_OVERRIDE;
    virtual void RetainTransport(char *);
    virtual void ReleaseTransport(char *);
    
    virtual IOReturn SetIdlePolicyValue(uint32_t idleTimeoutMs);
    virtual bool TransportWillReEnumerate();
    virtual void ConvertPowerFlagsToString(IOPMPowerFlags, char *);
    virtual void SetupTransportSCOParameters();
    virtual void DestroyTransportSCOParameters();
    virtual bool WaitForSystemReadyForSleep(char *);
    virtual IOReturn StartBluetoothSleepTimer();
    virtual void CancelBluetoothSleepTimer();
    virtual os_log_t CreateOSLogObject();
    
    virtual IOReturn setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;
    static IOReturn setPropertiesAction( OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual IOReturn setPropertiesWL( OSObject * properties );
    
    virtual IOReturn HardReset();

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_VERSION_11_0
    virtual void DumpTransportProviderState();
#endif
    
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 0);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 1);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 2);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 3);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 4);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 5);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 6);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 7);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 8);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 9);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 10);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 11);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 12);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 13);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 14);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 15);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 16);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 17);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 18);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 19);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 20);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 21);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 22);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostControllerTransport, 23);
    
protected:
    IOBluetoothHCIController * mBluetoothFamily; //136
    IOBluetoothHostController * mBluetoothController; //144
    IOService * mProvider; //152
    IOWorkLoop * mWorkLoop; //160
    IOCommandGate * mCommandGate; //168
    UInt16 mPowerMask; //176
    
    bool mUSBControllerSupportsSuspend; //178
    UInt8 mTerminateState; //179
    bool mLMPLoggingEnabled; //180
    bool mSystemOnTheWayToSleep; //181
    uint8_t reserved2; //182
    uint8_t reserved3; //183
    bool mConfiguredPM; //184
    UInt32 mSwitchBehavior; //188
    bool mHardwareInitialized; //192
    UInt32 mHardwareStatus; //196
    UInt32 mTerminateCounter; //200
    bool mLMPLoggingAvailable; //204
    void * mRefCon; //208
    IOBluetoothHCIControllerInternalPowerState mCurrentInternalPowerState; //216
    IOBluetoothHCIControllerInternalPowerState mPendingInternalPowerState; //220
    UInt16 mConrollerTransportType; //224
    bool mSupportNewIdlePolicy; //226
    bool mBuiltIn; //227
    UInt32 mLocationID; //228
    bool mSupportPowerOff; //232
    UInt32 mSleepType; //236
    bool mIsControllerActive; //240
    uint8_t reserved4; //241
    UInt32 unknown111; //244
    UInt8 reserved5; //248
    UInt8 reserved6; //249
    bool mSupportWoBT; //250
    UInt8 mCurrentPMMethod; //251
    UInt64 mTransportCounter; //256, retain/released in Retain/ReleaseTransport
    SInt16 mTransportOutstandingCalls; //264
    bool reserved7; //266
    IOTimerEventSource * mBluetoothSleepTimerEventSource; //272
    bool mBluetoothSleepTimerStarted; //280
    os_log_t mInternalOSLogObject; //288
    bool reserved8; //296
    UInt16 mUARTProductID; //298
    IOBluetoothACPIMethods * mACPIMethods; //304
    bool reserved10; //312
    struct ExpansionData
    {
        UInt16 reserved;
    };
    ExpansionData * mExpansionData; //320
};

#endif
