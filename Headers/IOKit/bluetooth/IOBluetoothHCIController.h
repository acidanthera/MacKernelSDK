/*
 * Copyright © 1998-2021 Apple Inc. All rights reserved.
 * Copyright © 2021 cjiang. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IOBLUETOOTHHCICONTROLLER_H
#define _IOKIT_IOBLUETOOTHHCICONTROLLER_H

#include <IOKit/IOService.h>
#include <IOKit/bluetooth/Bluetooth.h>
#include <IOKit/usb/IOUSBHostDevice.h>
#include <os/log.h>

class IODisplayWrangler;
class IOWorkQueue;
class IOBluetoothHostController;
class IOBluetoothHostControllerTransport;
class IOBluetoothPacketLogger;
class IOBluetoothACPIMethods;

struct BluetoothHardwareListType
{
    IOBluetoothHostControllerTransport  * mBluetoothTransport; //0
    IOBluetoothHostController           * mBluetoothHostController; //8
    //16
    UInt8 mHardResetCounter; //19
    //20
    BluetoothHardwareListType           * mNextHardware; //24
    BluetoothHardwareListType           * mPreviousHardware; //32
};

struct HCIEventListener
{
    task_t owningTask;
    mach_port_t port;
    void * refCon;
    UInt64 unknown;
};

class IOBluetoothHCIController : public IOService
{
    OSDeclareDefaultStructors(IOBluetoothHCIController)
    
    friend class IOBluetoothHostControllerTransport;
    friend class IOBluetoothHostControllerUSBTransport;
    friend class IOBluetoothHostControllerUARTTransport;
    
public:
    virtual bool init( OSDictionary * dictionary = NULL ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    static IOReturn stopAction( OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3 );
    virtual IOReturn stopWL( IOService * provider );
    virtual bool terminate( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    static IOReturn terminateAction( OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3 );
    virtual IOReturn terminateWL( IOOptionBits options = 0 );
    
    virtual IOReturn newUserClient(task_t owningTask, void * securityID, UInt32 type, LIBKERN_RETURNS_RETAINED IOUserClient ** handler) APPLE_KEXT_OVERRIDE;
    virtual IOReturn AddCommandUserClient(void * userClient);
    virtual void DetachUserClients();
    
    virtual IOReturn setProperties( OSObject * properties ) APPLE_KEXT_OVERRIDE;
    static IOReturn setPropertiesAction(OSObject *, void *, void *, void *, void *);
    virtual IOReturn setPropertiesWL( OSObject * properties );
    virtual bool ValidProperty( OSSymbol * );

    virtual void terminateServiceNubs();
    virtual bool terminateServiceNubsComplete();
    
    static void USBHardResetEntry(OSObject *); //this one is weird, as it is called by thread_call_allocate but is not in the right format
    static bool PacketLoggerPublishNotificationHandler(void *, void *, IOService *, IONotifier *);
    static bool HandlePacketLoggerPublishNotification(void *, IOService *);
    static bool DisplayManagerPublishNotificationHandler(void *, void *, IOService *, IONotifier *);
    static bool HandleDisplayManagerPublishNotification(void *, IOService *);
    static bool ExternalDisplayTerminateNotificationHandler(void *, void *, IOService *, IONotifier *);
    static bool HandleExternalDisplayTerminateNotification(void *, IOService *);
    static bool ExternalDisplayPublishNotificationHandler(void *, void *, IOService *, IONotifier *);
    static bool HandleExternalDisplayPublishNotification(void *, IOService *);
    static bool staticBluetoothTransportGoesAway(void *, void *, IOService *, IONotifier *);
    static bool staticBluetoothTransportShowsUp(void *, void *, IOService *, IONotifier *);
    
    virtual IOReturn AddHCIEventNotification( task_t inOwningTask, mach_port_t inPort, void * refCon );
    virtual IOReturn RemoveHCIEventNotification( task_t inOwningTask );
    
    virtual bool GetNvramPacketLoggerBufferSize(UInt32 *);
    virtual bool NeedToWaitForControllerToShowUp();
    
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    virtual IOCommandGate * getCommandGate() const;
    virtual IOWorkLoop * getUSBHardResetWorkLoop() const;
    virtual IOCommandGate * getUSBHardResetCommandGate() const;
    
    virtual void LogPacket( UInt8 packetType, void * packetData, size_t packetSize );
    
    virtual bool shouldOverrideExistingController(IOBluetoothHCIController *, BluetoothHardwareListType *);
    virtual IOReturn SwitchToSelectedHostController(UInt32);
    
    static IOReturn ProcessBluetoothTransportGoesAwayAction(IOBluetoothHCIController *, UInt8 *, UInt32);
    virtual IOReturn ProcessBluetoothTransportGoesAwayActionWL(IOBluetoothHostControllerTransport *);
    static IOReturn ProcessBluetoothTransportShowsUpAction(IOBluetoothHCIController *, UInt8 *, UInt32);
    virtual IOReturn ProcessBluetoothTransportShowsUpActionWL(IOBluetoothHostControllerTransport *);
    
    virtual IOReturn SetBluetoothTransportTerminateState(IOBluetoothHostControllerTransport *, UInt8);
    virtual IOReturn GetControllerDeviceAddress(IOBluetoothHostController *, BluetoothDeviceAddress *);
    
    virtual BluetoothHardwareListType * FindFirstBluetoothHardwareHasTransport();
    virtual BluetoothHardwareListType * FindBluetoothHardware(BluetoothDeviceAddress *);
    virtual BluetoothHardwareListType * FindBluetoothHardware(UInt16, UInt16, UInt32);
    virtual BluetoothHardwareListType * FindBluetoothHardware(IOBluetoothHostController *);
    virtual BluetoothHardwareListType * FindBluetoothHardware(IOBluetoothHostControllerTransport *);
    virtual BluetoothHardwareListType * FindBluetoothHardware(BluetoothHardwareListType *);
    virtual BluetoothHardwareListType * FindBluetoothHardware(UInt32);
    
    virtual IOBluetoothHostController * FindInternalBluetoothController();
    
    virtual IOReturn AddBluetoothHardware(BluetoothHardwareListType *);
    virtual IOReturn RemoveBluetoothHardware(BluetoothHardwareListType *, bool);
    virtual IOReturn SwitchBluetoothHardware(BluetoothHardwareListType *);
    virtual void PrintBluetoothHardwareList(bool);
    
    virtual bool SearchForTransportEventTimeOutHandler(); //?
    virtual bool WriteActiveControllerInfoToNVRAM(UInt16 productID, UInt16 vendorID, UInt8 * deviceAddress, UInt32 locationID, UInt16 activeConnections);
    virtual bool ReadActiveControllerInfoFromNVRAM(UInt16 * productID, UInt16 * vendorID, UInt8 * deviceAddress, UInt32 * locationID, UInt16 * activeConnections);
    virtual IOReturn UpdateNVRAMControllerInfo();
    virtual bool WriteInternalControllerInfoToNVRAM(UInt16, UInt16, UInt8 *, UInt32);
    virtual bool ReadInternalControllerInfoFromNVRAM(UInt16 *, UInt16 *, UInt8 *, UInt32 *);
    virtual IOReturn CreateBluetoothHostControllerObject(BluetoothHardwareListType *);
    virtual IOReturn RemoveBluetoothHostControllerObject(BluetoothHardwareListType *);
    virtual IOReturn DisableBluetoothHostControllerObject(BluetoothHardwareListType *);
    virtual IOReturn EnableBluetoothHostControllerObject(BluetoothHardwareListType *);
    
    virtual void WakeUpDisplay();
    virtual void FullWakeTimeOutHandler();
    virtual IOReturn StartFullWakeTimer();
    virtual void CancelFullWakeTimer();
    
    virtual bool IsTBFCSupported();
    virtual bool IsAnyDevicesTBFCPageCapable();
    virtual bool SupportDeepIdle();
    virtual bool isSystemPortable();
    virtual void SetIdlePolicy(void *, bool);
    virtual bool BluetoothRemoteWakeEnabled();
    virtual bool G3StandbySleepWithFileVaultOn();
    
    virtual bool SetActiveControllerInfoToPropertyList(UInt16 productID, UInt16 vendorID, UInt8 * deviceAddress, UInt32 locationID, UInt16 activeConnections);
    virtual bool AllControllersFinishedSettingUp();
    virtual IOReturn FamilyCommandSleep(void *, UInt32, char *, bool);
    virtual UInt32 GetCurrentTime();
    virtual UInt32 ConvertAddressToUInt32(void *);
    virtual bool ActiveControllerOnline();
    virtual bool SpecialWakeReason();
    
    virtual bool setProperty(const OSSymbol * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const OSString * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, const char * aString) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, bool aBoolean) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, unsigned long long aValue, unsigned int aNumberOfBits) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, void * bytes, unsigned int length) APPLE_KEXT_OVERRIDE;
    
    virtual bool SetBluetoothFamilyProperty(const OSSymbol * aKey, OSObject * anObject);
    virtual bool SetBluetoothFamilyProperty(const OSString * aKey, OSObject * anObject);
    virtual bool SetBluetoothFamilyProperty(const char * aKey, OSObject * anObject);
    virtual bool SetBluetoothFamilyProperty(const char * aKey, const char * aString);
    virtual bool SetBluetoothFamilyProperty(const char * aKey, bool aBoolean);
    virtual bool SetBluetoothFamilyProperty(const char * aKey, unsigned long long aValue, unsigned int aNumberOfBits);
    virtual bool SetBluetoothFamilyProperty(const char * aKey, void * bytes, unsigned int length);
    
    virtual bool UpdateActiveControllerProperties(char *);
    virtual UInt32 GetNextBluetoothObjectID();
    virtual IOReturn FreeBluetoothObjectID(UInt32);
    virtual void CheckACPIMethodsAvailabilities();
    virtual IOReturn CallRegisterService(void);
    
    virtual bool ReachHardResetLimit(IOBluetoothHostController *);
    virtual void IncrementHardResetCounter(IOBluetoothHostController *);
    virtual void ResetHardResetCounter(IOBluetoothHostController *);
    virtual UInt8 GetHardResetCounter(IOBluetoothHostController *);
    
    virtual IOReturn USBHardReset();
    static IOReturn USBHardResetAction(OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3);
    virtual void USBHardResetWL();
    virtual void ReleaseAllHardResetObjects();
    virtual bool FindBroadcomUSBHostDevice();
    virtual bool USBBluetoothModuleWithROMBootCapability();
    virtual IOReturn RecoverX238EModule(BluetoothHardwareListType *);
    
    virtual void RecoveryTimeOutHandler();
    virtual IOReturn StartRecoveryTimer();
    virtual void CancelRecoveryTimer();
    
    virtual IOReturn DumpStats();
    virtual void BeginSignPost();
    virtual void EndSignPost();
    
    virtual void ConvertErrorCodeToString(UInt32 errorCode, char * outStringLong, char * outStringShort);
    virtual void ConvertOpCodeToString(UInt16 opCode, char * outString);
    virtual void ConvertEventCodeToString(UInt8 eventCode, char * outString);
    virtual void ConvertVendorSpecificEventCodeToString(UInt8 eventCode, char * outString);
    virtual void ConvertEventStatusToString(UInt8 eventStatus, char * outString);
    
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 0);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 1);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 2);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 3);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 4);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 5);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 6);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 7);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 8);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 9);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 10);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 11);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 12);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 13);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 14);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 15);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 16);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 17);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 18);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 19);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 20);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 21);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 22);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 23);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 24);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 25);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 26);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 27);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 28);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 29);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 30);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 31);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 32);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 33);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 34);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 35);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 36);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 37);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 38);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 39);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 40);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 41);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 42);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 43);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 44);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 45);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 46);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 47);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 48);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 49);
    OSMetaClassDeclareReservedUnused(IOBluetoothHCIController, 50);
    
protected:
    IOWorkLoop * mWorkLoop; //136
    IOCommandGate * mCommandGate; //144
    IOWorkLoop * mUSBHardResetWorkLoop; //152
    IOCommandGate * mUSBHardResetCommandGate; //160
    OSSet * mUserClientSet; //168
    IOBluetoothPacketLogger * mPacketLogger; //176
    IONotifier * mPacketLoggerPublishNotifier; //184
    IODisplayWrangler * mDisplayManager; //192
    IONotifier * mDisplayManagerPublishNotifier; //200
    IONotifier * mExternalDisplayPublishNotifier; //208
    IONotifier * mExternalDisplayTerminateNotifier; //216
    
    bool mSearchForTransportEventTimerHasTimeout; //224
    IOTimerEventSource * mSearchForTransportEventTimer; //232
    bool mFullWakeWithAppleExternalDisplay; //240
    bool mExternalDisplayPublished; //241
    
    HCIEventListener * mHCIEventListenersList; //248
    IOByteCount mHCIEventListenersListSize; //256
    bool mDebugMode; //264
    
    IOWorkQueue * mFamilyWorkQueue; //272
    IOWorkQueue * mExternalControllerWorkQueue; //280
    BluetoothHardwareListType * mBluetoothHardwareListHead; //288
    BluetoothHardwareListType * mBluetoothHardwareListTail; //296
    BluetoothHardwareListType * mActiveBluetoothHardware; //304
    BluetoothHardwareListType * mActiveControllerBluetoothHardware; //312
    //320
    IONotifier * mBluetoothTransportShowsUpNotifier; //328
    IONotifier * mBluetoothTransportGoesAwayNotifier; //336
    bool mReceivedTransportNotification; //344
    //345
    UInt16 mActiveControllerProductID; //346
    UInt16 mActiveControllerVendorID; //348
    BluetoothDeviceAddress mActiveControllerAddress; //350
    UInt32 mActiveControllerLocationID; //356
    UInt16 mActiveControllerActiveConnections; //360
    bool mActiveControllerValid; //362
    //363
    
    UInt16 mInternalControllerProductID; //364
    UInt16 mInternalControllerVendorID; //366
    UInt8 xxxx; //368
    //369
    UInt16 cc; //372
    //374
    UInt32 mInternalControllerLocationID; //376
    bool mInternalControllerValid; //380
    bool mControllersFinishedSettingUp; //381
    //382
    bool gg; //383
    
    UInt32 mCheckACPIMethodsAvailabilitiesCallTime; //388
    //392
    //396
    //400
    UInt32 mActivityTickleCallTime; //404
    IOTimerEventSource * mFullWakeTimer; //408
    //416
    bool mFullWakeTimerHasTimeout; //417
    //418
    bool mSignPostStarted; //419
    os_log_t mInternalOSLogObject; //424
    //432
    //433
    //434
    bool mRegisterServiceCalled; //435
    
    thread_call_t mHardResetThreadCall;//440
    UInt32 mUSBHardResetWLCallTime; //448
    bool mTestHCICommandTimeoutHardReset; //453
    bool mTestNoHardResetWhenSleepCommandTimeout; //455
    bool mTestHCICommandTimeoutWhenWake; //456
    UInt8 d; //458
    UInt64 e; //464
    IOUSBHostDevice * mHardResetUSBHostDevice; //472
    IOUSBHostDevice * mHardResetUSBHub; //480
    IOBluetoothACPIMethods * mACPIMethods; //488
    IOTimerEventSource * mRecoveryTimer; //496
    bool mRecoveryTimerHasTimeout; //504
    //505
    UInt16 mCurrentBluetoothObjectID; //506
    bool mBluetoothObjects[0xFFFD]; //508
    IOLock * mBluetoothObjectsLock; //66048
    UInt16 wahtt; //66056
    //66057 UInt8
    struct ExpansionData
    {
        void * ptr1;
        void * ptr2;
    };
    ExpansionData * mExpansionData; //66064
};

#endif

/*
_IOBluetoothRingBufferRead
__IOBluetoothRingBufferBytesInBuffer
__IOBluetoothRingBufferSequentialBytesInBuffer
_IOBluetoothRingBufferReadAtOffset
_IOBluetoothRingBufferWrite
__IOBluetoothRingBufferBytesAvailable
__IOBluetoothRingBufferSequentialBytesAvailable
_IOBluetoothRingBufferWriteAtOffset
SearchForTransportEventTimeOutOccurred(OSObject *,IOTimerEventSource *);
FullWakeTimeOutOccurred(OSObject *,IOTimerEventSource *);
*/