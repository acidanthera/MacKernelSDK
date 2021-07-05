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

#ifndef _IOKIT_BLUETOOTH_IOBLUETOOTHHOSTCONTROLLER_H
#define _IOKIT_BLUETOOTH_IOBLUETOOTHHOSTCONTROLLER_H

#include <IOKit/IOService.h>
#include <IOKit/bluetooth/Bluetooth.h>
#include <IOKit/bluetooth/IOBluetoothInternal.h>
#include <IOKit/IOReportTypes.h>
#include <os/log.h>

class IOBluetoothHostControllerUserClient;
class IOBluetoothHostControllerTransport;
class IOBluetoothHCIController;
class IOBluetoothHCIRequest;
class IOWorkQueue;
class IOReportLegend;
class IOStateReporter;
class IOSimpleReporter;
class IOBluetoothDevice;
class BluetoothDeviceReporter;
class IOBluetoothInactivityTimerEventSource;
class IOBluetoothACLMemoryDescriptor;
class IOBluetoothSCOMemoryDescriptorRetainer;

struct BluetoothBroadcomBFCReconnectData;
struct BluetoothBroadcomBFCParams;
struct BluetoothBroadcomBFCRemoteBPCSFeatures;
struct BluetoothBroadcomLocalFirmwareInfo;
struct BluetoothBroadcomSetEventMask;
struct BluetoothBroadcomBFCConnectionTBFCSuspendedInfo;
struct BluetoothBroadcomVerboseConfigVersionInfo;

enum
{
    kRadioPowerInitialState      = 0,
    kRadioPoweringOff            = 1,
    kRadioPoweringOn             = 2,
    kRadioPoweredOff             = 3,
    kRadioPoweredOn              = 4,
    kRadioPowerError             = 5
};

typedef uint16_t DevicePublishNotificationStateType;
enum DevicePublishNotificationStateTypes
{
    kHasNotRegisteredForDevicePublishNotification        = 0x0001,
    kHasRegisteredForDevicePublishNotification           = 0x0002,
    kDevicePublishNotificationCalled                     = 0x0004,
    kDevicePublishNotificationProcessed                  = 0x0008
};

typedef UInt32 HCIDataHandlerType;
enum HCIDataHandlerTypes
{
    kTransportDataTypeHCIEvents,
    kTransportDataTypeACL,
    kTransportDataTypeSCO
};

typedef struct HearingDeviceListType
{
    BluetoothDeviceAddress                  mDeviceAddress;
    bool                                    mRemoveDeviceCalled;
    DevicePublishNotificationStateType      mDevicePublishNotificationState;
    HearingDeviceListType *                 mNextDevice;
    HearingDeviceListType *                 mPreviousDevice;
} HearingDeviceListType;

typedef struct LEDeviceListType
{
    BluetoothConnectionHandle               mConnectionHandle;
    bool unknown;
    LEDeviceListType *                      mNextDevice;
    LEDeviceListType *                      mPreviousDevice;
} BluetoothLEDevice;

class IOBluetoothHostController : public IOService
{
    OSDeclareDefaultStructors(IOBluetoothHostController)
    
    typedef IOReturn (*IOBluetoothIncomingDataAction) (IOBluetoothHostController * hostController, UInt8 * inDataPtr, UInt32 inDataSize, UInt32 inSequenceNumber);
    
    friend class IOBluetoothHostControllerTransport;
    friend class IOBluetoothHostControllerUSBTransport;
    friend class IOBluetoothHostControllerUARTTransport;
    
public:
    virtual IOReturn newUserClient(task_t owningTask, void * securityID, UInt32 type, IOUserClient ** handler) APPLE_KEXT_OVERRIDE;
    virtual IOReturn AddUserClient(void *);
    virtual void DetachUserClients();
    
    virtual bool InitializeVariables(IOBluetoothHostControllerTransport * transport);
    virtual bool init(IOBluetoothHCIController * family, IOBluetoothHostControllerTransport * transport);
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    virtual bool SetVariables();
    virtual void Enable(IOBluetoothHostControllerTransport * transport);
    virtual void Disable();
    virtual void StopAndFreeVariables();
    
    virtual bool start(IOService * provider) APPLE_KEXT_OVERRIDE;
    virtual void stop(IOService * provider) APPLE_KEXT_OVERRIDE;
    static IOReturn stopAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);
    virtual IOReturn stopWL(IOService * provider);
    
    virtual bool terminate(IOOptionBits options = 0) APPLE_KEXT_OVERRIDE;
    static IOReturn terminateAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);
    virtual IOReturn terminateWL(IOOptionBits options = 0);
    
    virtual IOReturn setProperties(OSObject * properties) APPLE_KEXT_OVERRIDE;
    static IOReturn setPropertiesAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);
    virtual IOReturn setPropertiesWL(OSObject * properties);
    
    virtual bool setProperty(const OSSymbol * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const OSString * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, OSObject * anObject) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, const char * aString) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, bool aBoolean) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, unsigned long long aValue, unsigned int aNumberOfBits) APPLE_KEXT_OVERRIDE;
    virtual bool setProperty(const char * aKey, void * bytes, unsigned int length) APPLE_KEXT_OVERRIDE;
    
    virtual bool SetBluetoothControllerProperty(const OSSymbol * aKey, OSObject * anObject);
    virtual bool SetBluetoothControllerProperty(const OSString * aKey, OSObject * anObject);
    virtual bool SetBluetoothControllerProperty(const char * aKey, OSObject * anObject);
    virtual bool SetBluetoothControllerProperty(const char * aKey, const char * aString);
    virtual bool SetBluetoothControllerProperty(const char * aKey, bool aBoolean);
    virtual bool SetBluetoothControllerProperty(const char * aKey, unsigned long long aValue, unsigned int aNumberOfBits);
    virtual bool SetBluetoothControllerProperty(const char * aKey, void * bytes, unsigned int length);
    
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    virtual IOCommandGate * getCommandGate() const;
    
    virtual void BroadcastEventNotification(BluetoothHCIRequestID inID, BluetoothHCIEventCode inEventCode, IOReturn eventStatus, UInt8 * inDataToSendPtr, IOByteCount inDataSize, BluetoothHCICommandOpCode inOpCode, bool, UInt8);
    virtual void BroadcastNotification(BluetoothHCIRequestID inID, IOBluetoothHCIControllerConfigState oldState, IOBluetoothHCIControllerConfigState newState);
    
    virtual void WaitForBluetoothd();
    virtual void FoundBluetoothd();
    
    virtual IOBluetoothHCIControllerFeatureFlags GetControllerFeatureFlags();
    virtual bool SetControllerFeatureFlags(IOBluetoothHCIControllerFeatureFlags featureFlags);
    
    virtual void setConfigState(IOBluetoothHCIControllerConfigState configState);
    
    virtual bool InitializeController();
    virtual IOReturn SetupController(bool *);
    virtual IOReturn SetupGeneralController();
    virtual UInt32 SetupCommonHardware();
    virtual bool ControllerSetupIsComplete();
    virtual void ControllerSetupComplete(int);
    virtual void BluetoothHostControllerSetupCompleted();
    virtual bool InitializeHostControllerVariables(bool);
    virtual IOReturn CallInitializeHostControllerVariables();
    virtual void CompleteInitializeHostControllerVariables();
    
    virtual IOReturn configureReport(IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport(IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination) APPLE_KEXT_OVERRIDE;
    
    virtual bool CreatePowerReporters();
    virtual IOReturn UpdatePowerReports(IOBluetoothHCIControllerInternalPowerState powerState);
    virtual IOReturn CallCreateDeviceReporter(IOBluetoothDevice * device);
    static IOReturn CreateDeviceReporterAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
    virtual IOReturn CreateDeviceReporter(IOBluetoothDevice * device);
    virtual BluetoothDeviceReporter * FindDeviceReporter(const BluetoothDeviceAddress * address);
    virtual IOReturn ConvertClassOfDeviceToDeviceType(UInt32, bool, UInt16 *, char *, UInt64 *, char *, UInt8 *);
    virtual IOReturn AddDeviceReporter(BluetoothDeviceReporter * reporter);
    virtual IOReturn RemoveDeviceReporter(IOBluetoothDevice * device);
    virtual IOReturn UpdateDeviceReporter(IOBluetoothDevice * device, bool);
    virtual IOReturn GetDeviceCounterForName(UInt16, char *, char *, UInt8 *);
    virtual void PrintDeviceReporterList();
    virtual IOReturn UpdateLESetAdvertisingDataReporter(IOBluetoothHCIRequest * request);
    virtual IOReturn SetupPowerStateReporter();
    
    virtual void DesyncIncomingData(IOBluetoothIncomingDataAction action, UInt8 * inDataPtr, UInt32 inDataSize);
    static void DesyncIncomingDataAction(IOBluetoothHostController * hostController, IOBluetoothIncomingDataAction action, void * inDataPtr, UInt32 inDataSize, UInt32 sequenceNumber);
    virtual void SynchronizePacketSequence(UInt32 sequenceNumber);
    virtual void SynchronizeSCOPacketSequence(UInt32 sequenceNumber);
    
    virtual IOBluetoothDevice * FindDeviceWithHandle(BluetoothConnectionHandle inConnectionHandle);
    virtual IOBluetoothDevice * FindDeviceWithSCOHandle(BluetoothConnectionHandle inConnectionHandle);
    virtual IOBluetoothDevice * FindDeviceWithAddress(const BluetoothDeviceAddress * inDeviceAddress, bool);
    
    virtual HearingDeviceListType * FindHearingDeviceWithAddress(const BluetoothDeviceAddress * inDeviceAddress);
    virtual IOReturn AddHearingDevice(IOBluetoothDevice * inDevice);
    virtual IOReturn RemoveHearingDevice(IOBluetoothDevice *inDevice, bool all);
    
    virtual LEDeviceListType * FindLEDeviceWithConnectionHandle(BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn AddLEDevice(BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn RemoveLEDevice(BluetoothConnectionHandle inConnectionHandle, bool all);
    
    virtual IOReturn AddDevice( IOBluetoothDevice * inDevice );
    virtual IOReturn RemoveDevice( IOBluetoothDevice * inDevice );
    virtual void PrintDeviceList();
    
    virtual IOReturn SetDevicePublishNotificationState(const BluetoothDeviceAddress *, UInt16);
    virtual UInt16 GetDevicePublishNotificationState(const BluetoothDeviceAddress *);
    virtual IOReturn CreateDeviceFromLEConnectionResults(BluetoothHCIEventLEConnectionCompleteResults * connectionResults);
    virtual IOReturn CreateDeviceFromConnectionResults(BluetoothHCIEventConnectionCompleteResults * connectionResults);
    virtual IOReturn CreateDeviceFromConnectionResults(BluetoothHCIEventConnectionCompleteResults * connectionResults, bool isInitiator);
    virtual IOReturn DestroyDeviceWithDisconnectionResults(BluetoothHCIEventDisconnectionCompleteResults * disconnectionResults);
    virtual IOReturn DestroyDevice(IOBluetoothDevice * inDevice);
    virtual IOReturn DestroyAllDevices();
    
    virtual bool terminateServiceNubs();
    virtual bool terminateServiceNubsComplete();
    
    virtual void FlushDeviceACLPackets(IOBluetoothDevice * inDevice);
    virtual void SaveNumOutstandingACLPackets();
    virtual void DecrementIdleTimerActivityCount(UInt16);
    virtual void IncrementIdleTimerActivityCount();
    virtual void DecrementOutstandingACLPackets(UInt16, UInt16, UInt16, UInt16, UInt16, UInt16); //Gonna be useful
    
    virtual void ProcessFlushOccurredEvent(BluetoothHCIEventFlushOccurredResults * inFlushResults);
    virtual void ProcessNumberOfCompletedPacketsEvent(UInt8 *);
    virtual void ProcessHCIControllerResetEvent();
    
    virtual IOReturn DispatchIncomingACLData(UInt8 * inDataPtr, UInt32 inDataSize);
    virtual IOReturn DispatchIncomingSCOData(UInt8 * inDataPtr, UInt32 inDataSize, UInt32 inMissingData, AbsoluteTime inTimestamp);
    virtual IOBluetoothDevice * OpenDeviceConnection(const BluetoothDeviceAddress * inDeviceAddress);
    static IOReturn OpenDeviceConnectionAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4 );
    virtual IOBluetoothDevice * OpenDeviceConnectionWL(const BluetoothDeviceAddress * inDeviceAddress);
    virtual IOReturn SendACLData(IOMemoryDescriptor * memDescriptor);
    static void ACLPacketTimerFired(OSObject * owner, IOTimerEventSource * timerEventSource);
    virtual IOReturn SendACLPacket(IOBluetoothACLMemoryDescriptor * memDescriptor, IOBluetoothDevice * inTargetDevice = NULL);
    virtual IOReturn TransferACLPacketToHW(IOMemoryDescriptor * memDescriptor);
    virtual void handleACLPacketTimeout(IOBluetoothACLMemoryDescriptor *);
    virtual IOReturn setUnackQueueCompletionCalled(void *);
    static IOReturn setUnackQueueCompletionCalledAction(OSObject * onwer, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
    
    virtual IOReturn SetACLPacketCompletion(void *);
    virtual IOReturn EnqueuePacket(IOMemoryDescriptor * memDescriptor, IOBluetoothDevice * inTargetDevice);
    virtual IOReturn DequeuePacket(UInt16, UInt16, UInt16, UInt16, UInt16, UInt16);
    virtual IOReturn RemovePacket(IOBluetoothACLMemoryDescriptor *);
    virtual UInt32 RemoveAllPacketsBelongingTo(IOBluetoothDevice *); //returns number of packets removed
    virtual IOReturn AdjustACLQueuesLimits(IOBluetoothDevice *);
    virtual IOReturn AdjustACLQueuesAllowance(IOBluetoothDevice *, bool);
    static IOReturn AdjustACLQueuesAllowanceAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
    virtual char * CreateACLQueueDebugString(char *, UInt16);
    virtual void SendSCOCompleted(IOBluetoothSCOMemoryDescriptorRetainer *, AbsoluteTime timestamp);
    static IOReturn SendSCOCompletedAction(OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
    virtual IOReturn SendSCOData(IOMemoryDescriptor *, IOBluetoothSCOMemoryDescriptorRetainer *, int);
    virtual IOReturn SendSCOData(IOMemoryDescriptor *);
    static void SCOPacketTimerFired(OSObject * owner, IOTimerEventSource * timerEventSource);
    virtual void handleSCOPacketTimeout(IOBluetoothSCOMemoryDescriptorRetainer *);
    
    virtual void SetNumSCOConnections(UInt8, UInt32);
    virtual IOReturn ToggleSCOConnection();
    virtual IOReturn ToggleeSCOEV3Connection();
    virtual IOReturn ToggleeSCOEV4Connection();
    virtual IOReturn ToggleeSCOEV5Connection();
    
    virtual UInt16 getSynchronousConnectionPacketTypeProperty();
    
    virtual bool NeedToSetupController();
    static IOReturn NeedToSetupControllerAction(IOBluetoothHostController *);
    virtual bool FullWake(char *);
    
    virtual void ProcessLMPData(UInt8 * data, UInt32 dataSize);
    virtual IOReturn ProcessLMPDataAction(IOBluetoothHostController * hostController, UInt8 * data, UInt32 dataSize);
    virtual void ProcessLMPDataWL(UInt8 * data, UInt32 dataSize);
    
    virtual void ProcessACLData(UInt8 * data, UInt32 dataSize);
    static IOReturn ProcessACLDataAction(IOBluetoothHostController * hostController, UInt8 * incomingDataPtr, UInt32 inDataSize, UInt32 inSequenceNumber);
    virtual void ProcessACLDataWL(UInt8 * data, UInt32 dataSize, UInt32 sequenceNumber);
    
    virtual void ProcessSCOData(UInt8 * data, UInt32 dataSize, UInt32 inMissingData, AbsoluteTime inTimestamp, Boolean copyData);
    static IOReturn ProcessSCODataAction(IOBluetoothHostController * hostController, UInt8 * incomingDataPtr, UInt32 inDataSize, UInt32 inMissingData, AbsoluteTime * inTimestamp, Boolean inCopyData );
    virtual void ProcessSCODataWL(UInt8 * data, UInt32 dataSize, UInt32 inMissingData, AbsoluteTime inTimestamp, Boolean copyData);
    
    virtual void ProcessEventData(UInt8 * inDataPtr, UInt32 inDataSize);
    static IOReturn ProcessEventDataAction(IOBluetoothHostController * hostController, UInt8 * incomingDataPtr, UInt32 inDataSize, UInt32 inSequenceNumber);
    virtual void ProcessEventDataWL(UInt8 * inDataPtr, UInt32 inDataSize, UInt32 sequenceNumber);
    virtual void NotifyDataClientsForEventData(IOBluetoothHCIRequest * hciRequest, UInt32, UInt8 *);
    
    virtual bool GetCompleteCodeForCommand(BluetoothHCICommandOpCode inOpCode, BluetoothHCIEventCode * outEventCode);
    virtual IOReturn GetOpCodeAndEventCode(UInt8 * inDataPtr, UInt32 inDataSize, BluetoothHCICommandOpCode * outOpCode, BluetoothHCIEventCode * eventCode, BluetoothHCIEventStatus * outStatus, UInt8 *, BluetoothDeviceAddress * outDeviceAddress, BluetoothConnectionHandle * outConnectionHandle, bool *);
    
    virtual IOReturn FindConnectionCompleteType(BluetoothDeviceAddress * inDeviceAddress, BluetoothHCICommandOpCode * outOpCode);
    virtual IOReturn FindSynchronousConnectionCompleteType(BluetoothDeviceAddress * inDeviceAddress, BluetoothHCICommandOpCode * outOpCode);
    
    virtual IOReturn HandleSpecialOpcodes(BluetoothHCICommandOpCode opCode);
    virtual IOReturn HCIRequestCreate(BluetoothHCIRequestID * outRequestID, bool inDoAsyncNotify = true, UInt32 inTimeout = 5, BluetoothHCIRequestCallbackInfo * inCallbackInfo = NULL, task_t inTaskID = 0, UInt32 inControlFlags = 0);
    virtual IOReturn HCIRequestDelete(task_t inTask, BluetoothHCIRequestID inID);
    virtual IOReturn LookupRequest(BluetoothHCIRequestID inID, IOBluetoothHCIRequest ** outRequestPtr);
    virtual IOReturn PrepareRequestForNewCommand(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inDeviceAddress, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn DisposeRequestsForTaskID(task_t inTaskID);
    virtual IOReturn CleanUpOrphanedHCIRequest();
    virtual void CleanUpExpiredHCIRequest();
    virtual bool HCIRequestQueuesAreEmpty();
    virtual void RemoveAllHCIRequestBelongsToThisDevice(IOBluetoothDevice * thisDevice);
    virtual bool HCIRequestIsSynchronous(UInt32, bool *);
    
    virtual bool SetHCIRequestRequireEvents(UInt16, IOBluetoothHCIRequest * request);
    virtual bool SetHCIRequestReceivedEvents(UInt8, IOBluetoothHCIRequest * request);
    virtual void ValidateHCIRequestReceivedEvents(UInt8, IOBluetoothHCIRequest * request);
    
    virtual IOReturn SendHCIRequestFormatted(BluetoothHCIRequestID inID, BluetoothHCICommandOpCode inOpCode, IOByteCount outResultsSize, void * outResultsPtr, const char * inFormat, ...);
    virtual IOReturn SendRawHCICommand(BluetoothHCIRequestID inID, char * buffer, IOByteCount bufferSize, UInt8 *, IOByteCount);
    virtual IOReturn ProcessWaitingRequests(bool);
    virtual IOReturn SendingRequest(IOBluetoothHCIRequest * requestPtr);
    virtual IOReturn SendHCIRequestToTransport(UInt8 *, IOByteCount);
    
    virtual IOReturn EnqueueRequestForController(IOBluetoothHCIRequest * requestPtr);
    virtual IOReturn EnqueueRequest(IOBluetoothHCIRequest * inRequestPtr);
    virtual IOReturn DequeueRequest(IOBluetoothHCIRequest * inRequestPtr);
    virtual IOReturn FindQueuedRequest(BluetoothHCICommandOpCode opCode, BluetoothDeviceAddress * inDeviceAddress, BluetoothConnectionHandle inConnectionHandle, Boolean inUseAttributes, IOBluetoothHCIRequest ** outRequestPtr);
    virtual IOReturn EnqueueWaitRequest(IOBluetoothHCIRequest * inRequestPtr);
    virtual IOReturn DequeueWaitRequest(IOBluetoothHCIRequest * inRequestPtr);
    virtual IOReturn FindQueuedWaitRequest(BluetoothHCICommandOpCode opCode, IOBluetoothHCIRequest ** outRequestPtr);
    virtual IOReturn AbortRequest(IOBluetoothHCIRequest * inRequestPtr);
    virtual IOReturn AbortRequestAndSetTime(IOBluetoothHCIRequest *);
    virtual IOReturn KillAllPendingRequests(bool destroy, bool includeIdleRequests);
    virtual IOReturn CallKillAllPendingRequests(bool destroy, bool includeIdleRequests);;
    virtual IOReturn IncrementHCICommandTimeOutCounter();
    virtual void ResetHCICommandTimeOutCounter();
    
    virtual void IncrementActiveConnections();
    virtual void DecrementActiveConnections();
    virtual void ActiveConnectionsInProgress();
    virtual void AllConnectionsGone();
    virtual void MergeChannelDescription(OSDictionary * destination , OSDictionary * source);
    virtual void RemoveChannelRestrictions(OSDictionary * toChange , OSDictionary * removeRule, const char * key);
    virtual bool IsAllowedDevice(OSDictionary * description , IOBluetoothDevice * device);
    virtual void RemoveAllRules();

    virtual OSDictionary * GetIncomingL2CAPChannelDescription(OSNumber * psmNumber);
    virtual IOReturn AddAllowedIncomingL2CAPChannel(OSDictionary * channelDescription);
    virtual void RemoveAllowedIncomingL2CAPChannel(OSObject * channelID);

    virtual void AddAllowedIncomingL2CAPChannel(BluetoothL2CAPPSM incomingPSM);
    virtual void AddAllowedIncomingL2CAPChannel(OSNumber * psmNumber);
    virtual void RemoveAllowedIncomingL2CAPChannel(BluetoothL2CAPPSM incomingPSM);

    virtual OSDictionary * GetIncomingRFCOMMChannelDescription(OSNumber * channelIDNumber);
    virtual IOReturn AddAllowedIncomingRFCOMMChannel(OSDictionary * channelDescription);
    virtual void AddAllowedIncomingRFCOMMChannel(BluetoothRFCOMMChannelID incomingChannelID);
    virtual void AddAllowedIncomingRFCOMMChannel(OSNumber * channelIDNumber);
    virtual void AddSecureIncomingRFCOMMChannel(BluetoothRFCOMMChannelID incomingChannelID);
    virtual void AddSecureIncomingRFCOMMChannel(OSNumber * channelIDNumber);
    virtual void RemoveAllowedIncomingRFCOMMChannel(OSObject * channelID);
    virtual void RemoveAllowedIncomingRFCOMMChannel(BluetoothRFCOMMChannelID incomingChannelID);
    virtual bool IsSecureIncomingRFCOMMChannel(BluetoothRFCOMMChannelID incomingChannelID);
    virtual void SetEnabledIncomingRFCOMMChannel(BluetoothRFCOMMChannelID incomingChannelID, bool ShouldBeEnabled);
    
    virtual bool IsAllowedIncomingL2CAPChannelForDevice(BluetoothL2CAPPSM incomingPSM, IOBluetoothDevice * device);
    virtual bool IsAllowedIncomingRFCOMMChannelForDevice(BluetoothRFCOMMChannelID incomingChannelID, IOBluetoothDevice * device);
    
    virtual void StartIdleTimer(uint32_t milliseconds);
    static void idleTimerFired(OSObject * owner, IOTimerEventSource * timerEventSource);
    virtual void StopIdleTimer();
    virtual void EvaluateHighPriorityConnections();
    virtual void handleIdleTimeout();
    virtual IOReturn CallCancelTimeoutForIdleTimer();
    virtual IOReturn CallUpdateTimerForIdleTimer();
    virtual IOReturn CallResetTimerForIdleTimer();
    virtual IOReturn EnableIdleTimer();
    virtual IOReturn DisableIdleTimer();
    virtual UInt32 ChangeIdleTimerTime(char *, UInt32);
    virtual IOReturn SetIdleTimerValue(UInt32);
    virtual IOReturn SetIdleTimerValueInNVRAM(UInt32);
    virtual IOReturn ClearIdleTimerValueInNVRAM();
    virtual void WriteIdleTimerValueToNVRAM();
    virtual void ReadIdleTimerValueFromNVRAM();
    virtual void RemoveIdleTimerValueInNVRAM();
    
    virtual bool SetTransportIdlePolicyValue();
    virtual void WaitForAllDevicesToGetReadyToBeDestroyed();
    
    virtual bool UpdatePowerStateProperty(IOBluetoothHCIControllerInternalPowerState powerState, bool);
    virtual IOReturn RequestPowerStateChange( IOBluetoothHCIControllerInternalPowerState newPowerState, char *);
    virtual IOReturn RequestPowerStateChangeFromUserSpace(IOBluetoothHCIControllerInternalPowerState newPowerState);
    virtual bool Power_State_Change_In_Progress();
    virtual bool Power_State_Is_Off();
    virtual bool Is_Powering_Down();
    virtual IOBluetoothHCIControllerInternalPowerState GetControllerPowerState();
    virtual IOReturn GetTransportCurrentPowerState(IOBluetoothHCIControllerInternalPowerState * outPowerState);
    virtual IOReturn GetTransportPendingPowerState(IOBluetoothHCIControllerInternalPowerState * outPowerState);
    virtual IOReturn ChangeTransportPowerStateTo(UInt64, bool, IOBluetoothHCIControllerInternalPowerState, char *);
    virtual IOReturn WaitForTransportPowerStateChange(IOBluetoothHCIControllerInternalPowerState, char *);
    virtual bool SystemReadyForSleep();
    virtual void SetChangingPowerState(bool);
    virtual void SetControllerPowerOptions(IOBluetoothHCIControllerPowerOptions inPowerOptions);
    virtual IOBluetoothHCIControllerPowerOptions GetControllerPowerOptions();
    virtual void TransportIsReady(bool);
    virtual bool TransportIsGoingAway();
    virtual IOReturn ChangeControllerStateForRestart();
    virtual IOReturn CleanUpForPoweringOff(); //not sure
    virtual IOReturn CleanUpBeforeTransportTerminate(IOBluetoothHostControllerTransport * transport);
    virtual void SetVendorSpecificEventMask(bool);
    virtual IOReturn CleanUpForCompletePowerChangeFromSleepToOn();
    virtual IOReturn CleanupForPowerChangeFromOnToSleep(bool, UInt32 *);
    
    virtual IOReturn PerformTaskForPowerManagementCalls(IOReturn);
    virtual IOReturn disableHIDEmulation();
    virtual IOReturn SetHIDEmulation(UInt32, bool);
    virtual IOReturn CallSetHIDEmulation();
    virtual bool ModuleIsInUHEMode(bool *);
    virtual IOReturn TransportRadioPowerOff(UInt16, char *, int, IOBluetoothHCIRequest *);
    virtual IOReturn GetTransportRadioPowerState(UInt8 *);
    virtual IOReturn SetTransportRadioPowerState(UInt8);
    virtual IOReturn CallPowerRadio(bool);
    virtual IOReturn SendBFCSetParams(UInt8, UInt8, UInt8);
    virtual bool SetMaxPowerForConnection(UInt32, UInt8, UInt16);
    virtual IOReturn WriteDeviceAddress(UInt32, BluetoothDeviceAddress *);
    virtual IOReturn WriteLocalSupportedFeatures(UInt32, void *);
    virtual IOReturn WriteBufferSize();
    virtual bool DisableScan(); //not sure
    virtual IOReturn CallBluetoothHCIReset(bool, char *);
    virtual IOReturn IgnoreUSBReset(bool);
    virtual IOReturn HardResetController(UInt16); //useful
    virtual bool WillResetModule();
    virtual IOReturn ResetTransportHardwareStatus();
    virtual IOReturn GetTransportHardwareStatus(UInt32 *);
    virtual void TransportTerminating(IOBluetoothHostControllerTransport *);
    virtual IOReturn GetTransportInfo(BluetoothTransportInfo *);
    virtual bool SupportWoBT();
    virtual bool IsTBFCSupported();
    virtual bool IsAnyDevicesTBFCPageCapable();
    virtual bool SupportDeepIdle();
    virtual IOReturn SetIdlePolicy(bool);
    virtual bool BluetoothRemoteWakeEnabled();
    virtual bool FileVaultIsTurnedOn();
    virtual IOReturn ControllerCommandSleep(void *, UInt32, char *, bool);
    
    virtual UInt32 ConvertAddressToUInt32(void * inAddress);
    virtual IOReturn UpdateFirmware(UInt32);
    virtual bool NeedToIncreaseInactivityTime();
    virtual void DisableControllerWorkQueue();
    virtual void DoNotTickleDisplay();
    virtual IOReturn ValidSleepTypeFromPowerManager(UInt32 *);
    virtual IOReturn ToggleLMPLogging();
    virtual IOReturn SetupLighthouse();
    virtual IOReturn SetLighthouseControl(UInt8);
    virtual IOReturn SetLighthouseParameters(UInt16, UInt16, UInt16, UInt8, UInt8, SInt8, UInt8);
    virtual IOReturn SetLighthouseDebugQuery(UInt8);
    virtual void WakeUpLEConnectionCompleteOutOfSequenceThread();
    virtual IOReturn RecoverX238EModule();
    virtual void TakeAHexDump(const void * inData, UInt32 inDataSize);
    virtual void PrintAllHCIRequests();
    virtual void PrintDeviceListAddress();
    virtual void PrintClassVariablesContent();
    virtual os_log_t CreateOSLogObject();
    virtual BluetoothHCIRequestID GetMaxRequestID(); //return 500
    virtual IOReturn TestLEAdvertisingReportEvent(UInt8);
    virtual bool IsClassicSecureConnectionsSupported();
    
    virtual IOReturn BluetoothHCIInquiry(BluetoothHCIRequestID inID, BluetoothLAP inLAP, BluetoothHCIInquiryLength inInquiryLength, BluetoothHCIResponseCount inMaxResponseCount, BluetoothHCIInquiryResults * outResults);
    virtual IOReturn BluetoothHCIInquiryCancel(BluetoothHCIRequestID inID);
    virtual IOReturn BluetoothHCIPeriodicInquiryMode(BluetoothHCIRequestID inID, BluetoothHCIInquiryLength inMaxPeriodLength, BluetoothHCIInquiryLength inMinPeriodLength, BluetoothLAP inLAP, BluetoothHCIInquiryLength inInquiryLength, BluetoothHCIResponseCount inMaxResponses, BluetoothHCIInquiryResults * outResults);
    virtual IOReturn BluetoothHCIExitPeriodicInquiryMode(BluetoothHCIRequestID inID);
    virtual IOReturn BluetoothHCICreateConnection(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothPacketType inPacketType, BluetoothPageScanRepetitionMode inPageScanRepetitionMode, BluetoothPageScanMode inPageScanMode, BluetoothClockOffset inClockOffset, BluetoothAllowRoleSwitch inAllowRoleSwitch, BluetoothHCIEventConnectionCompleteResults * outConnectionHandle);
    virtual IOReturn BluetoothHCIDisconnect(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothReasonCode inReason, BluetoothHCIEventDisconnectionCompleteResults * outResults);
    virtual IOReturn BluetoothHCIAddSCOConnection(BluetoothHCIRequestID inID, BluetoothConnectionHandle inACLConnectionHandle, BluetoothPacketType inPacketType);
    virtual IOReturn BluetoothHCIAcceptConnectionRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothRole inRole);
    virtual IOReturn BluetoothHCIRejectConnectionRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothReasonCode inReason);
    virtual IOReturn BluetoothHCILinkKeyRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, const BluetoothKey * inKeyPtr, BluetoothDeviceAddress * outAddress);
    virtual IOReturn BluetoothHCILinkKeyRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothDeviceAddress * outAddress);
    virtual IOReturn BluetoothHCIPINCodeRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, IOByteCount inPINCodeSize, const BluetoothPINCode * inPINCode, BluetoothDeviceAddress * outAddress);
    virtual IOReturn BluetoothHCIPINCodeRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothDeviceAddress * outAddress);
    virtual IOReturn BluetoothHCIChangeConnectionPacketType(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothPacketType inPacketType);
    virtual IOReturn BluetoothHCIAuthenticationRequested(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCISetEncryptionEnable(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothEncryptionEnable inEncryptionEnable);
    virtual IOReturn BluetoothHCIChangeConnectionLinkKey(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCIMasterLinkKey(BluetoothHCIRequestID inID, BluetoothKeyFlag inKeyFlag);
    virtual IOReturn BluetoothHCIRemoteNameRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothPageScanRepetitionMode inPageScanRepetitionMode, BluetoothPageScanMode inPageScanMode, BluetoothClockOffset inClockOffset, BluetoothHCIEventRemoteNameRequestResults * outName);
    virtual IOReturn BluetoothHCIReadRemoteSupportedFeatures(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIEventReadRemoteSupportedFeaturesResults * outFeatures);
    virtual IOReturn BluetoothHCIReadRemoteExtendedFeatures(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIExtendedInquiryResponseDataType inDataType, BluetoothHCIEventReadRemoteExtendedFeaturesResults * outFeatures);
    virtual IOReturn BluetoothHCIReadRemoteVersionInformation(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIVersionInfo * outVersionInfo);
    virtual IOReturn BluetoothHCIReadClockOffset(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothClockOffset * outClockOffset);
    
    virtual IOReturn BluetoothHCIIOCapabilityRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, UInt8, UInt8, UInt8);
    virtual IOReturn BluetoothHCIUserConfirmationRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr);
    virtual IOReturn BluetoothHCIUserConfirmationRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr);
    virtual IOReturn BluetoothHCIUserPasskeyRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, UInt32);
    virtual IOReturn BluetoothHCIUserPasskeyRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr);
    virtual IOReturn BluetoothHCIRemoteOOBDataRequestReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothHCISimplePairingOOBData *, BluetoothHCISimplePairingOOBData *);
    virtual IOReturn BluetoothHCIRemoteOOBDataRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr);
    virtual IOReturn BluetoothHCIIOCapabilityRequestNegativeReply(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, UInt8);
    virtual IOReturn BluetoothHCISetupSynchronousConnection(BluetoothHCIRequestID inID, UInt16, BluetoothHCISetupSynchronousConnectionParams *);
    virtual IOReturn BluetoothHCIAcceptSynchronousConnectionRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothHCIAcceptSynchronousConnectionRequestParams *);
    virtual IOReturn BluetoothHCIRejectSynchronousConnectionRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, UInt8);
    virtual IOReturn BluetoothHCIEnhancedSetupSynchronousConnection(BluetoothHCIRequestID inID, UInt16, BluetoothHCIEnhancedSetupSynchronousConnectionParams *);
    virtual IOReturn BluetoothHCIEnhancedAcceptSynchronousConnectionRequest(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, BluetoothHCIEnhancedAcceptSynchronousConnectionRequestParams *);
    
    virtual IOReturn BluetoothHCIHoldMode(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIModeInterval inHoldModeMaxInterval, BluetoothHCIModeInterval inHoldModeMinInterval);
    virtual IOReturn BluetoothHCISniffMode(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIModeInterval inSniffModeMaxInterval, BluetoothHCIModeInterval inSniffModeMinInterval, BluetoothHCISniffAttemptCount inSniffAttemptCount, BluetoothHCISniffTimeout inSniffModeTimeout);
    virtual IOReturn BluetoothHCIExitSniffMode(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCIParkMode(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIParkModeBeaconInterval inMaxInterval, BluetoothHCIParkModeBeaconInterval inMinInterval);
    virtual IOReturn BluetoothHCIExitParkMode(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCIQualityOfServiceSetup(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIQualityOfServiceSetupParams * inSetupPtr);
    virtual IOReturn BluetoothHCIRoleDiscovery(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIRoleInfo * outRoleInfo);
    virtual IOReturn BluetoothHCISwitchRole(BluetoothHCIRequestID inID, BluetoothDeviceAddress * inAddressPtr, BluetoothHCIRole inNewRole);
    virtual IOReturn BluetoothHCIReadLinkPolicySettings(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCILinkPolicySettingsInfo * outSettingsInfo);
    virtual IOReturn BluetoothHCIReadDefaultLinkPolicySettings(BluetoothHCIRequestID inID, BluetoothHCILinkPolicySettingsInfo * outSettingsInfo);
    virtual IOReturn BluetoothHCIWriteLinkPolicySettings(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCILinkPolicySettings inSettings, BluetoothConnectionHandle * outConnectionHandle);
    virtual IOReturn BluetoothHCIWriteDefaultLinkPolicySettings(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCISniffSubrating(BluetoothHCIRequestID inID, UInt16, UInt16, UInt16, UInt16, UInt16 *);
    virtual IOReturn BluetoothHCIFlowSpecification(BluetoothHCIRequestID inID, BluetoothHCIEventFlowSpecificationData *, BluetoothHCIEventFlowSpecificationData *);
    virtual IOReturn BluetoothHCISetEventMask(BluetoothHCIRequestID inID, BluetoothSetEventMask * inMask);
    virtual IOReturn BluetoothHCILESetEventMask(BluetoothHCIRequestID inID, BluetoothSetEventMask * inMask);
    virtual IOReturn BluetoothHCILEReadBufferSize(BluetoothHCIRequestID inID, BluetoothHCILEBufferSize * outSize);
    virtual IOReturn BluetoothHCILESetScanEnable(BluetoothHCIRequestID inID, UInt8, UInt8);
    virtual IOReturn BluetoothHCILESetAdvertiseEnable(BluetoothHCIRequestID inID, UInt8);
    virtual IOReturn BluetoothHCILEReadLocalSupportedFeatures(BluetoothHCIRequestID inID, BluetoothHCISupportedFeatures * outFeatures);
    virtual IOReturn BluetoothHCILEReadRemoteUsedFeatures(BluetoothHCIRequestID inID, UInt16, BluetoothHCISupportedFeatures * outFeatures);
    virtual IOReturn BluetoothHCIReset(BluetoothHCIRequestID inID);
    virtual IOReturn BluetoothHCISetEventFilter(BluetoothHCIRequestID inID, UInt8, UInt8, BluetoothEventFilterCondition *);
    virtual IOReturn BluetoothHCIFlush(BluetoothHCIRequestID inID, BluetoothConnectionHandle inHandle);
    virtual IOReturn BluetoothHCIReadPINType(BluetoothHCIRequestID inID, BluetoothPINType * outType);
    virtual IOReturn BluetoothHCIWritePINType(BluetoothHCIRequestID inID, BluetoothPINType inType);
    
    virtual IOReturn BluetoothHCICreateNewUnitKey(BluetoothHCIRequestID inID);
    virtual IOReturn BluetoothHCIReadStoredLinkKey(BluetoothHCIRequestID inID, BluetoothDeviceAddress * targetDevice, BluetoothHCIReadStoredLinkKeysFlag * inFlags, BluetoothHCIStoredLinkKeysInfo * outKeysInfo);
    virtual IOReturn BluetoothHCIWriteStoredLinkKey(BluetoothHCIRequestID inID, IOItemCount inNumKeysToWrite, BluetoothDeviceAddress inDeviceAddresses[], BluetoothKey inLinkKeys[], BluetoothHCINumLinkKeysToWrite * outNumKeysWritten);
    virtual IOReturn BluetoothHCIDeleteStoredLinkKey(BluetoothHCIRequestID inID, BluetoothDeviceAddress * targetDevice, BluetoothHCIDeleteStoredLinkKeyFlag * inFlag, BluetoothHCINumLinkKeysDeleted * outNumDeleted);
    
    virtual IOReturn BluetoothHCIChangeLocalName(BluetoothHCIRequestID inID, BluetoothDeviceName newName);
    virtual IOReturn BluetoothHCIReadLocalName(BluetoothHCIRequestID inID, BluetoothDeviceName name);
    virtual IOReturn BluetoothHCIReadConnectionAcceptTimeout(BluetoothHCIRequestID inID, BluetoothHCIConnectionAcceptTimeout * outTimeout);
    virtual IOReturn BluetoothHCIWriteConnectionAcceptTimeout(BluetoothHCIRequestID inID, BluetoothHCIConnectionAcceptTimeout inTimeout);
    virtual IOReturn BluetoothHCIReadPageTimeout(BluetoothHCIRequestID inID, BluetoothHCIPageTimeout * outDataPtr);
    virtual IOReturn BluetoothHCIWritePageTimeout(BluetoothHCIRequestID inID, BluetoothHCIPageTimeout inTimeout);
    virtual IOReturn BluetoothHCIReadScanEnable(BluetoothHCIRequestID inID, BluetoothHCIPageScanEnableState * outState);
    virtual IOReturn BluetoothHCIWriteScanEnable(BluetoothHCIRequestID inID, BluetoothHCIPageScanEnableState inState, bool);
    virtual IOReturn BluetoothHCIReadPageScanActivity(BluetoothHCIRequestID inID, BluetoothHCIScanActivity * outActivityInfo);
    virtual IOReturn BluetoothHCIWritePageScanActivity(BluetoothHCIRequestID inID, BluetoothHCIScanActivity * inActivityInfo);
    virtual IOReturn BluetoothHCIReadInquiryScanActivity(BluetoothHCIRequestID inID, BluetoothHCIScanActivity * outActivityInfo);
    virtual IOReturn BluetoothHCIWriteInquiryScanActivity(BluetoothHCIRequestID inID, BluetoothHCIScanActivity * inActivityInfo);
    virtual IOReturn BluetoothHCIReadAuthenticationEnable(BluetoothHCIRequestID inID, BluetoothHCIAuthenticationEnable * outAuthenticationState);
    virtual IOReturn BluetoothHCIWriteAuthenticationEnable(BluetoothHCIRequestID inID, BluetoothHCIAuthenticationEnable inAuthenticationState);
    
    virtual IOReturn BluetoothHCIReadEncryptionMode(BluetoothHCIRequestID inID, BluetoothHCIEncryptionMode * outEncryptionState);
    virtual IOReturn BluetoothHCIWriteEncryptionMode(BluetoothHCIRequestID inID, BluetoothHCIEncryptionMode inEncryptionMode);
    virtual IOReturn BluetoothHCIReadClassOfDevice(BluetoothHCIRequestID inID, BluetoothClassOfDevice * outClassOfDevice);
    virtual IOReturn BluetoothHCIWriteClassOfDevice(BluetoothHCIRequestID inID, BluetoothClassOfDevice inClassOfDevice);
    virtual IOReturn BluetoothHCIReadVoiceSetting(BluetoothHCIRequestID inID, BluetoothHCIVoiceSetting * outVoiceSetting);
    virtual IOReturn BluetoothHCIWriteVoiceSetting(BluetoothHCIRequestID inID, BluetoothHCIVoiceSetting inVoiceSetting);
    virtual IOReturn BluetoothHCIReadAutomaticFlushTimeout(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIAutomaticFlushTimeoutInfo * outAutomaticFlushTimeoutInfo);
    virtual IOReturn BluetoothHCIWriteAutomaticFlushTimeout(BluetoothHCIRequestID inID, BluetoothHCIAutomaticFlushTimeoutInfo * inAutomaticFlushTimeoutInfo, BluetoothConnectionHandle * outConnectionHandle);
    virtual IOReturn BluetoothHCIReadNumBroadcastRetransmissions(BluetoothHCIRequestID inID, BluetoothHCINumBroadcastRetransmissions * outNumRetrans);
    virtual IOReturn BluetoothHCIWriteNumBroadcastRetransmissions(BluetoothHCIRequestID inID, BluetoothHCINumBroadcastRetransmissions inNumRetrans);
    virtual IOReturn BluetoothHCIReadHoldModeActivity(BluetoothHCIRequestID inID, BluetoothHCIHoldModeActivity * outState);
    virtual IOReturn BluetoothHCIWriteHoldModeActivity(BluetoothHCIRequestID inID, BluetoothHCIHoldModeActivity inState);
    virtual IOReturn BluetoothHCIReadTransmitPowerLevel(BluetoothHCIRequestID inID, BluetoothConnectionHandle inHandle, BluetoothHCITransmitPowerLevelType inType, BluetoothHCITransmitPowerLevelInfo * outLevelInfo);
    virtual IOReturn BluetoothHCIReadSCOFlowControlEnable(BluetoothHCIRequestID inID, BluetoothHCIFlowControlState * outState);
    virtual IOReturn BluetoothHCIWriteSCOFlowControlEnable(BluetoothHCIRequestID inID, BluetoothHCIFlowControlState inState);
    virtual IOReturn BluetoothHCISetHostControllerToHostFlowControl(BluetoothHCIRequestID inID, BluetoothHCIFlowControlState inState);
    virtual IOReturn BluetoothHCIHostBufferSize(BluetoothHCIRequestID inID, BluetoothHCIBufferSize * inSize);
    virtual IOReturn BluetoothHCIHostNumberOfCompletePackets(BluetoothHCIRequestID inID, UInt8, UInt16 *, UInt16 *);
    virtual IOReturn BluetoothHCIReadLinkSupervisionTimeout(BluetoothHCIRequestID inID, BluetoothConnectionHandle inHandle, BluetoothHCILinkSupervisionTimeout * outInfo);
    virtual IOReturn BluetoothHCIWriteLinkSupervisionTimeout(BluetoothHCIRequestID inID, BluetoothHCILinkSupervisionTimeout * inInfo, BluetoothConnectionHandle * outHandle);
    virtual IOReturn BluetoothHCIReadNumberOfSupportedIAC(BluetoothHCIRequestID inID, BluetoothHCISupportedIAC * outNumSupported);
    virtual IOReturn BluetoothHCIReadCurrentIACLAP(BluetoothHCIRequestID inID, BluetoothHCICurrentInquiryAccessCodes * inAccessCodes);
    virtual IOReturn BluetoothHCIWriteCurrentIACLAP(BluetoothHCIRequestID inID, BluetoothHCICurrentInquiryAccessCodesForWrite * inAccessCodes);
    virtual IOReturn BluetoothHCIReadPageScanPeriodMode(BluetoothHCIRequestID inID, BluetoothHCIPageScanPeriodMode * outMode);
    virtual IOReturn BluetoothHCIWritePageScanPeriodMode(BluetoothHCIRequestID inID, BluetoothHCIPageScanPeriodMode inMode);
    virtual IOReturn BluetoothHCIReadPageScanMode(BluetoothHCIRequestID inID, BluetoothHCIPageScanMode * outMode);
    virtual IOReturn BluetoothHCIWritePageScanMode(BluetoothHCIRequestID inID, BluetoothHCIPageScanMode inMode);

    virtual IOReturn BluetoothHCIEnhancedFlush(BluetoothHCIRequestID inID, UInt16, UInt8, UInt16 *);
    virtual IOReturn BluetoothHCIReadExtendedInquiryResponse(BluetoothHCIRequestID inID, BluetoothHCIReadExtendedInquiryResponseResults *);
    virtual IOReturn BluetoothHCIWriteExtendedInquiryResponse(BluetoothHCIRequestID inID, UInt8, BluetoothHCIExtendedInquiryResponse *);
    virtual IOReturn BluetoothHCIRefreshEncryptionKey(BluetoothHCIRequestID inID, UInt16, BluetoothHCIEventEncryptionKeyRefreshCompleteResults *);
    virtual IOReturn BluetoothHCIReadInquiryMode(BluetoothHCIRequestID inID, BluetoothHCIInquiryMode *);
    virtual IOReturn BluetoothHCIWriteInquiryMode(BluetoothHCIRequestID inID, BluetoothHCIInquiryMode);
    virtual IOReturn BluetoothHCIReadSimplePairingMode(BluetoothHCIRequestID inID, BluetoothHCISimplePairingMode *);
    virtual IOReturn BluetoothHCIWriteSimplePairingMode(BluetoothHCIRequestID inID, BluetoothHCISimplePairingMode);
    virtual IOReturn BluetoothHCIReadLocalOOBData(BluetoothHCIRequestID inID, BluetoothHCIReadLocalOOBDataResults *);
    virtual IOReturn BluetoothHCIReadInquiryResponseTransmitPower(BluetoothHCIRequestID inID, BluetoothHCITransmitPowerLevel *);
    virtual IOReturn BluetoothHCIWriteInquiryResponseTransmitPower(BluetoothHCIRequestID inID, BluetoothHCITransmitPowerLevel);
    virtual IOReturn BluetoothHCISendKeypressNotification(BluetoothHCIRequestID inID, const BluetoothDeviceAddress * inAddressPtr, UInt8);
    virtual IOReturn BluetoothHCIReadDefaultErroneousDataReporting(BluetoothHCIRequestID inID, BluetoothHCIErroneousDataReporting *);
    virtual IOReturn BluetoothHCIWriteDefaultErroneousDataReporting(BluetoothHCIRequestID inID, BluetoothHCIErroneousDataReporting);
    virtual IOReturn BluetoothHCIReadAFHChannelAssessmentMode(BluetoothHCIRequestID inID, BluetoothHCIAFHChannelAssessmentMode *);
    virtual IOReturn BluetoothHCIWriteAFHChannelAssessmentMode(BluetoothHCIRequestID inID, BluetoothHCIAFHChannelAssessmentMode);
    virtual IOReturn BluetoothHCISetAFHHostChannelClassification(BluetoothHCIRequestID inID, BluetoothAFHHostChannelClassification *);
    virtual IOReturn BluetoothHCIReadInquiryScanType(BluetoothHCIRequestID inID, BluetoothHCIInquiryScanType *);
    virtual IOReturn BluetoothHCIWriteInquiryScanType(BluetoothHCIRequestID inID, BluetoothHCIInquiryScanType);
    virtual IOReturn BluetoothHCIReadPageScanType(BluetoothHCIRequestID inID, BluetoothHCIPageScanType *);
    virtual IOReturn BluetoothHCIWritePageScanType(BluetoothHCIRequestID inID, BluetoothHCIPageScanType);
    
    virtual IOReturn BluetoothHCIReadLocalVersionInformation(BluetoothHCIRequestID inID, BluetoothHCIVersionInfo * outVersionInfo);
    virtual IOReturn BluetoothHCIReadLocalSupportedCommands(BluetoothHCIRequestID inID, BluetoothHCISupportedCommands * outCommands);
    virtual IOReturn BluetoothHCIReadLocalSupportedFeatures(BluetoothHCIRequestID inID, BluetoothHCISupportedFeatures * outFeatures);
    virtual IOReturn BluetoothHCIReadLocalExtendedFeatures(BluetoothHCIRequestID inID, UInt8, BluetoothHCIExtendedFeaturesInfo * outFeatures);
    virtual IOReturn BluetoothHCIReadBufferSize(BluetoothHCIRequestID inID, BluetoothHCIBufferSize * outSize);
    virtual IOReturn BluetoothHCIReadCountryCode(BluetoothHCIRequestID inID, BluetoothHCICountryCode * outCountryCode);
    virtual IOReturn BluetoothHCIReadDeviceAddress(BluetoothHCIRequestID inID, BluetoothDeviceAddress * outAddress);
    virtual IOReturn BluetoothHCIReadFailedContactCounter(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIFailedContactInfo * outFailedContactCount);
    virtual IOReturn BluetoothHCIResetFailedContactCounter(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCIGetLinkQuality(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCILinkQualityInfo * outLinkQualityInfo);
    virtual IOReturn BluetoothHCIReadRSSI(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIRSSIInfo * outRSSIInfo);
    virtual IOReturn BluetoothHCIReadAFHChannelMap(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothAFHResults * outAFHResults);
    virtual IOReturn BluetoothHCIReadClock(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, UInt8, BluetoothReadClockInfo * outClockInfo);
    virtual IOReturn BluetoothHCIReadEncryptionKeySize(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIEncryptionKeySizeInfo * outEncryptionKeySizeInfo);
    virtual IOReturn BluetoothHCICreateConnectionCancel(BluetoothHCIRequestID inID, const BluetoothDeviceAddress *, BluetoothDeviceAddress *);
    virtual IOReturn BluetoothHCIRemoteNameRequestCancel(BluetoothHCIRequestID inID, const BluetoothDeviceAddress *, BluetoothDeviceAddress *);
    virtual IOReturn BluetoothHCIReadLMPHandle(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIReadLMPHandleResults * outLMPHandleResults);
    
    virtual IOReturn BluetoothHCIReadLoopbackMode(BluetoothHCIRequestID inID, BluetoothHCILoopbackMode * inLoopbackMode);
    virtual IOReturn BluetoothHCIWriteLoopbackMode(BluetoothHCIRequestID inID, BluetoothHCILoopbackMode inLoopbackMode);
    virtual IOReturn BluetoothHCIEnableDeviceUnderTestMode(BluetoothHCIRequestID inID);
    virtual IOReturn BluetoothHCIWriteSimplePairingDebugMode(BluetoothHCIRequestID inID, BluetoothSimplePairingDebugMode inSimplePairingDebugMode);
    
    virtual IOReturn VendorCommand(BluetoothHCIRequestID inID, BluetoothHCIVendorCommandSelector inSelector, UInt8 * inCommandData, IOByteCount inCommandDataSize, IOByteCount outBufferSize, UInt8 * outBuffer);
    
    virtual IOReturn BluetoothHCIBroadcomReadRawRSSI(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, BluetoothHCIRSSIInfo * outRSSIInfo);
    virtual IOReturn BluetoothHCIBroadcomBFCSuspend(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle);
    virtual IOReturn BluetoothHCIBroadcomBFCResume(BluetoothHCIRequestID inID, BluetoothConnectionHandle inConnectionHandle, const BluetoothDeviceAddress * inAddressPtr, BluetoothBroadcomBFCReconnectData *);
    virtual IOReturn BluetoothHCIBroadcomBFCReadParams(UInt32, BluetoothBroadcomBFCParams *);
    virtual IOReturn BluetoothHCIBroadcomBFCSetParams(UInt32, BluetoothBroadcomBFCParams *);
    virtual IOReturn BluetoothHCIBroadcomSetTransmitPower(UInt32, UInt16, SInt8);
    virtual IOReturn BluetoothHCIBroadcomBFCReadRemoteBPCSFeatures(UInt32, UInt16, BluetoothBroadcomBFCRemoteBPCSFeatures *);
    virtual IOReturn BluetoothHCIBroadcomBFCWriteScanEnable(UInt32, UInt8);
    virtual IOReturn BluetoothHCIBroadcomBFCCreateConnection(UInt32, const BluetoothDeviceAddress *, UInt16, BluetoothHCIEventConnectionCompleteResults *);
    virtual IOReturn BluetoothHCIBroadcomSetEventMask(UInt32, BluetoothBroadcomSetEventMask *);
    virtual IOReturn BluetoothHCIBroadcomReadLocalFirmwareInfo(UInt32, UInt8, BluetoothBroadcomLocalFirmwareInfo *);
    virtual IOReturn BluetoothHCIBroadcomBFCReadScanEnable(UInt32, UInt8 *);
    virtual IOReturn BluetoothHCIBroadcomBFCIsConnectionTBFCSuspended(UInt32, UInt16, BluetoothBroadcomBFCConnectionTBFCSuspendedInfo *);
    virtual IOReturn BluetoothHCIBroadcomSetUSBAutoResume(UInt32, UInt16);
    virtual IOReturn BluetoothHCIBroadcomChangeLNAGainCoexsECI(UInt32, UInt8);
    virtual IOReturn BluetoothHCIBroadcomTurnOFFDynamicPowerControl(UInt32, UInt8, const BluetoothDeviceAddress *);
    virtual IOReturn BluetoothHCIBroadcomIncreaseDecreasePowerLevel(UInt32, const BluetoothDeviceAddress *, UInt8);
    virtual IOReturn BluetoothHCIBroadcomARMMemoryPoke(UInt32, UInt32, UInt32, UInt8 *);
    virtual IOReturn BluetoothHCIBroadcomARMMemoryPeek(UInt32, UInt32, UInt8 *);
    virtual IOReturn BluetoothHCIBroadcomUARTSetSleepmodeParam(UInt32, UInt8, UInt8, UInt8, UInt8, UInt8, UInt8, UInt8, UInt32, UInt8);
    virtual IOReturn BluetoothHCIBroadcomReadVerboseConfigVersionInfo(UInt32, BluetoothBroadcomVerboseConfigVersionInfo *);
    virtual IOReturn BluetoothHCIBroadcomGetBasicRateACLConnectionStats();
    virtual IOReturn BluetoothHCIBroadcomResetBasicRateACLConnectionStats();
    virtual IOReturn BluetoothHCIBroadcomGetEDRACLConnectionStats();
    virtual IOReturn BluetoothHCIBroadcomIgnoreUSBReset(UInt32, UInt8, UInt8 *);
    virtual IOReturn BluetoothHCIBroadcomLighthouseControl(UInt32, UInt8);
    virtual IOReturn BluetoothHCIBroadcomLighthouseSetParameters(UInt32, UInt16, UInt16, UInt16, UInt8, UInt8, SInt8, UInt8);
    virtual IOReturn BluetoothHCIBroadcomLighthouseDebugQuery(UInt32, UInt8);
    virtual IOReturn BluetoothHCIBroadcomMasterSkipSniffMode(UInt16, UInt8, UInt8, UInt16, UInt16);
    virtual IOReturn BluetoothHCIBroadcomLoadPwrRegulatoryFile(UInt8 *, UInt8);

private:
    static bool windowServerDidAppear( void * target, void * refCon, IOService * newService, IONotifier * notifier );
    static IOReturn windowServerDidAppearAction( OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5 );

    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 0);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 1);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 2);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 3);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 4);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 5);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 6);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 7);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 8);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 9);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 10);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 11);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 12);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 13);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 14);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 15);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 16);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 17);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 18);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 19);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 20);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 21);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 22);
    OSMetaClassDeclareReservedUnused(IOBluetoothHostController, 23);

protected:
    OSSet * mReporterSet; //136
    IOReportLegend * mReportLegend; //this + 144
    IOStateReporter * mPowerStateReporter; //this + 152
    BluetoothDeviceReporter ** mDeviceReporterList; //this + 160
    BluetoothDeviceReporter * mDeviceReporter; //this + 168
    IOSimpleReporter * mAppleBTLEAdvertisingReporter; //this + 176
    IOSimpleReporter * mLESetAdvertisingDataReporter; //this + 184
    void * x1; //this + 192
    IOWorkLoop * mWorkLoop; //this + 200
    IOCommandGate * mCommandGate; //this + 208
    IOBluetoothHCIRequest * mBusyQueueHead; //this + 216
    IOBluetoothHCIRequest * mWaitQueueHead; //this + 224
    IOBluetoothHCIRequest ** mHCIRequestList; //this + 232
    UInt64 mHCIRequestListSize; //this + 240
    UInt32 D248; //this + 248
    
    
    IOBluetoothDevice * mDeviceListHead; //this + 256
    void * u;//264, size is 2048
    void * uSize; //272
    UInt8 mNumberOfLowPriorityDevice; //280
    UInt8 mNumberOfMidPriorityDevice; //281
    UInt8 mNumberOfHighPriorityDevice; //282
    UInt8 mNumberOfLowPriorityLEDevice; //283
    UInt8 mNumberOfMidPriorityLEDevice; //284
    UInt8 mNumberOfHighPriorityLEDevice; //285
    BluetoothHCIBufferSize mACLSCOBufferSize; //286
    BluetoothHCILEBufferSize mLEACLBufferSize; //294
    
    UInt16 mNumberOfOutstandingLowPriorityACLPackets; //306
    UInt16 mNumberOfOutstandingMidPriorityACLPackets; //308
    UInt16 mNumberOfOutstandingHighPriorityACLPackets; //310
    
    UInt16 mTotalNumberOfOutstandingACLPackets; //312
    //314
    UInt32 mNumberOfLowPriorityACLPacketsInQueue; //316
    UInt32 mNumberOfMidPriorityACLPacketsInQueue; //320
    UInt32 mNumberOfHighPriorityACLPacketsInQueue; //324
    UInt32 mTotalNumberOfACLPacketsInAllQueues; //328
    UInt16 mNumberOfAllowedLowPriorityACLDataPackets; //332
    UInt16 mNumberOfAllowedMidPriorityACLDataPackets; //334
    UInt16 mNumberOfAllowedHighPriorityACLDataPackets; //336
    
    UInt16 mNumberOfOutstandingLowPriorityLEACLPackets; //400
    UInt16 mNumberOfOutstandingMidPriorityLEACLPackets; //402
    UInt16 mNumberOfOutstandingHighPriorityLEACLPackets; //404
    UInt16 mTotalNumberOfOutstandingLEACLPackets; //406
    UInt32 mNumberOfLowPriorityLEACLPacketsInQueue; //408
    UInt32 mNumberOfMidPriorityLEACLPacketsInQueue; //412
    UInt32 mNumberOfHighPriorityLEACLPacketsInQueue; //416
    UInt32 mTotalNumberOfLEACLPacketsInAllQueues; //420
    
    UInt16 mNumberOfAllowedLowPriorityLEACLDataPackets; //424
    UInt16 mNumberOfAllowedMidPriorityLEACLDataPackets; //426
    UInt16 mNumberOfAllowedHighPriorityLEACLDataPackets; //428
    
    
    
    IOBluetoothHCIControllerConfigState mPreviousControllerConfigState; //this + 504
    
    
    UInt8 * mSCOPacketBuffer; //this + 520
    
    IOBluetoothInactivityTimerEventSource * mIdleTimer; //544
    
    UInt16 mSynchronousConnectionPacketType; //this + 562

    HearingDeviceListType * mConnectedHearingDeviceListHead; //568
    HearingDeviceListType * mConnectedHearingDeviceListTail; //576
    
    UInt64 mTotalDataBytesSent; //728
    UInt64 mTotalDataBytesReceived; //736
    UInt64 mTotalSCOBytesReceived; //744
    bool mKillAllPendingRequestsCalled; //752
    
    
    OSSet * mHostControllerClients; //this + 800
    LEDeviceListType * mLEDeviceListHead; //this + 808
    LEDeviceListType * mLEDeviceListTail; //this + 816
    IOBluetoothHCIController * mBluetoothFamily; //this + 824
    IOBluetoothHostControllerTransport * mBluetoothTransport; //this + 832
    IOWorkQueue * mControllerWorkQueue; //this + 840
    IOWorkQueue * mReporterWorkQueue; //this + 848
    //856
    UInt16 mVendorID; //858
    UInt16 mProductID; //860
    
    bool mControllerSetupIsComplete; //this + 876
    bool mTransportIsReady; //this + 877
    bool mTransportIsGoingAway; //this + 878
    bool mIdlePolicyIsOneByte; //this + 886
    
    UInt8 mNumTimedOutHCICommands; //this + 889
    UInt16 xxxxxxx;
    UInt32 mTransportSleepType; //this + 892
    
    bool mCanDoHardReset; //896
    
    IOBluetoothHCIControllerPowerOptions mControllerPowerOptions; //this + 900
    IOBluetoothHCIControllerConfigState mControllerConfigState; //this + 904
    IOBluetoothHCIControllerFeatureFlags mControllerFeatureFlags; //this + 908
    UInt8 mNumberOfCommandsAllowedByHardware; //912
    
    UInt8 mNumSCOConnections; //914
    UInt64 mTotalSCOBytesSent; //920
    UInt16 mActiveConnections; //928
    
    UInt8 mNumConfiguredHIDDevices; //956
    bool unknown; //this + 959
    
    long long mLESetAdvertisingDataCommandSent[15]; //this + 1120 (types 1 - 15)
    //this + 1240
    
    
    bool mIdleTimerDisabled; //1248
    bool mPowerStateChanging; //1249
    
    UInt32 mIdleTimerValue; //1256
    bool un; //1260
    
    bool n; //1280
    SInt16 mControllerOutstandingCalls; //1284
    os_log_t mInternalOSLogObject; //1288
    
    bool mBluetoothdNotFound; //this + 1312
    
    struct ExpansionData
    {
        void * reserved;
    };
    ExpansionData * mIOBluetoothHostControllerExpansionData; //1320
};

#endif
