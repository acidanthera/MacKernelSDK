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

#ifndef _IO80211VIRTUALINTERFACE_H
#define _IO80211VIRTUALINTERFACE_H

#include <Availability.h>

#include <net/ethernet.h>
#include <net/kpi_interface.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <IOKit/80211/IO80211Controller.h>

#ifndef __MAC_OS_X_VERSION_MIN_REQUIRED
#error "Missing macOS target version"
#endif

class IO80211PeerManager;
class IO80211AsyncEventUserClient;
class IO80211P2PDaemonUserClient;

struct bpfTapCallback
{
    u_int32_t data_link_type;
    bpf_tap_mode direction;
    IO80211VirtualInterface * interface;
};

typedef UInt32 realTimeServiceId;

enum
{
    kIO80211VirtualInterfaceRoleP2PDevice                 = 0,
    kIO80211VirtualInterfaceRoleP2PClient                 = 1,
    kIO80211VirtualInterfaceRoleP2PGroupOwner             = 2,
    kIO80211VirtualInterfaceRoleAirLink                   = 3,
    kIO80211VirtualInterfaceRoleSoftAP                    = 4,
    kIO80211VirtualInterfaceRoleUknown                    = 5,
    kIO80211VirtualInterfaceRoleWiFiAwareDiscovery        = 6,
    kIO80211VirtualInterfaceRoleWiFiAwareDiscoveryAndData = 7,
    kIO80211VirtualInterfaceRoleWiFiAwareData             = 8,
    kIO80211VirtualInterfaceRoleUndefined
};

class IO80211VirtualInterface : public IOService
{
    OSDeclareDefaultStructors( IO80211VirtualInterface )

public:
    virtual bool init( IO80211Controller * controller, ether_addr_t * address, UInt32 role, char const * bsdName );
    virtual void free() APPLE_KEXT_OVERRIDE;
    
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_11_0
    virtual bool willTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
#endif
    virtual bool terminate( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    
    virtual bool attach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void detach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    IOReturn createIOReporters( IOService * service );
    virtual IOReturn configureReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport( IOReportChannelList * channels, IOReportUpdateAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    void reportTransmitStatus( mbuf_t packet, IOReturn status, struct packet_info_tx * info );
    IOReturn reportTransmitCompletionStatus( mbuf_t packet, IOReturn status, uint32_t param1 = 0, uint32_t param2 = 0, IOOptionBits options = 0 );
    
    bool reportDataPathEvents( UInt32 msg ,void * data = NULL, size_t dataSize = 0 );
    static IOReturn reportDataPathEventsGated( void * target, void * msg, void * data, void * dataSize, void * arg0 );
    
    void setAuthTimeout( AbsoluteTime timeout );
    AbsoluteTime authTimeout();
    
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual IOReturn newUserClient( task_t owningTask, void * securityID, UInt32 type, OSDictionary * properties, LIBKERN_RETURNS_RETAINED IOUserClient ** handler ) APPLE_KEXT_OVERRIDE;
#endif
    IOReturn resetUserClientReference();
    static IOReturn resetUserClientReferenceGated( OSObject * target, void * arg0, void * arg1, void * arg2, void * arg3 );
    IOReturn startAsyncEventUserClientForTask( task_t task, kIO80211InterfaceType );
    IOReturn startP2PDaemonUserClientForTask( task_t task );
    void p2pDaemonExited();
    
    virtual const char * stringFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;
    virtual int errnoFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;

    bool setInterfaceRole(UInt32 role);
    UInt32 getInterfaceRole();

    bool isPeerToPeerInterface();
    virtual bool createPeerManager( ether_addr * inAddress, IO80211PeerManager ** outManager );
    IO80211WorkLoop * getWorkLoop();
    const char * getBSDName();
    virtual IOMediumType getMediumType();
    IO80211Controller * getController();
    bool getInterfaceAddress(UInt8 *);
    
    virtual bool setLinkState( IO80211LinkState state, UInt32 reason );
    IO80211LinkState linkState();
    
    void setScanningState( UInt32 scanSource, bool scan, apple80211_scan_data * data, IOReturn status );
    void setJoiningState( UInt32 scanSource, joinStatus status, bool join ); // unimplemented
    void setInfraChannel(apple80211_channel * channel);
    void setInfraTxState( bool state );
    bool setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *,apple80211_awdl_sync_channel_sequence *);
    bool setInterfaceCCA( apple80211_channel, int, apple80211_awdl_sync_channel_sequence *);
    bool setInterfaceNF(apple80211_channel,long long);
    bool setInterfaceChipCounters(apple80211_stat_report *,apple80211_chip_counters_tx *,apple80211_chip_error_counters_tx *,apple80211_chip_counters_rx *);
    bool setInterfaceMIBdot11(apple80211_stat_report *,apple80211_ManagementInformationBasedot11_counters *);
    bool setFrameStats(apple80211_stat_report *,apple80211_frame_counters *);
    bool setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *);
    
    mbuf_t preQueuePacket(mbuf_t);
    void dropTxPacket( mbuf_t packet );
    UInt32 dequeueTxPackets( TxPacketRequest * tx );
    virtual IOReturn dequeueOutputPacketsWithServiceClass( UInt32 max, IOMbufServiceClass serviceClass, mbuf_t * firstPacket, mbuf_t * lastPacket, UInt32 * count, UInt64 * result );
    virtual UInt32 outputPacket( mbuf_t m, void * param );
    errno_t bpfOutputPacket(mbuf_t m, void *);
    errno_t bpfOutput(UInt32, mbuf_t m);
    
    virtual void setEnabledBySystem( bool state );
    bool enabledBySystem();
    
    static IOReturn ioctl_internal_gated(void * owner, void * request, void * arg0, void * arg1, void * arg2);
    IOReturn ioctl_internal(void * request);
    virtual errno_t handleIoctl(unsigned long inID, void * inData);
    static errno_t handleIoctlGated(void * owner, void * inID ,void * inData, void * arg0, void * arg1);
    errno_t handleSIOCGIFMEDIA(unsigned long inID, struct ifmediareq *);
    errno_t handleSIOCSIFADDR();
    errno_t handleSIOCSIFFLAGS( const char * source );
    
    virtual UInt32 inputPacket( mbuf_t packet, struct packet_info_tag * info );
    
    virtual IOReturn powerStateWillChangeTo( IOPMPowerFlags capabilities, unsigned long stateNumber, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    static IOReturn powerStateWillChangeToGated( void * owner, void * capabilities, void * stateNumber, void * whatDevice, void * arg );
    virtual IOReturn controllerWillChangePowerState(IO80211Controller * controller, IOPMPowerFlags capabilities, IO80211SystemPowerState stateNumber, IOService * whatDevice);
    
    virtual IOReturn powerStateDidChangeTo( IOPMPowerFlags capabilities, unsigned long stateNumber, IOService * whatDevice ) APPLE_KEXT_OVERRIDE;
    static IOReturn powerStateDidChangeToGated( void * owner, void * capabilities, void * stateNumber, void * whatDevice, void * arg );
    virtual IOReturn controllerDidChangePowerState(IO80211Controller * controller, IOPMPowerFlags capabilities, IO80211SystemPowerState stateNumber, IOService * whatDevice);
    
    virtual errno_t handleDebugCmd(apple80211_debug_command * command);
    
    IOReturn IO80211InterfacePostMessage(UInt32 msg ,void * data = NULL, size_t dataSize = 0);
    void postMessage( UInt32 msg, void * data = NULL, size_t dataSize = 0 );
    void postSyncStateChanged();
    errno_t postNewMasterElected();
    void postServiceIndication();
    void postAwdlStatistics( struct apple80211_awdl_statistics * ); // size = 2520
    void postAwdlHppStatsEvent(realTimeServiceId);
    void postLowlatencyStatistics(struct apple80211_lowlatency_peer_statistics_event *);
    void postAwdlAppSpecificInfo(struct apple80211_awdl_app_specific_info *);
    void postP2PAirplayStatistics(struct apple80211_p2p_airplay_statistics *);
    void postHostapChannelChanged(apple80211_hostap_state *);
    virtual void postPeerPresence(ether_addr * address, int32_t rssi, int linkQualityMetric, int nodeProximityMetric, char * serviceInfo);
    virtual void postPeerAbsence(ether_addr * address);
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual void postPeerPresenceIPv6(ether_addr * address, int32_t rssi, int linkQualityMetric, int nodeProximityMetric, char * serviceInfo, UInt8 * socketAddressData);
#endif
    virtual void signalOutputThread();
    virtual bool isOutputFlowControlled();
    virtual void setOutputFlowControlled();
    virtual void clearOutputFlowControlled();
    virtual IOReturn outputStart(IOOptionBits options);
    static IOReturn _outputStart(OSObject * owner, void * options, void * arg0, void * arg1, void * arg2);
    IOReturn _outputStartGated(IOOptionBits options);
    IOReturn _outputStartGatedNoPM(IOOptionBits options);
    
    virtual void configureAQMOutput();
    virtual void setUnitNumber(const char * name);
    void setUnitNumber(const char * name, UInt32 number);
    
    virtual void initIfnetEparams(struct ifnet_init_eparams * params);
    void ifnet_start_callback(ifnet_t ifnet);
    void ifnet_detach_callback(ifnet_t ifnet);
    void ifnet_ioctl_callback(ifnet_t ifnet, unsigned long id, void * data);
    
    virtual bool attachToBpf();
    bool peerToPeerAttachToBpf();
    errno_t bpfAttach( UInt32 dataLinkType, UInt32 headerLength, OSObject * target, IOOutputAction action, IOBPFTapAction tap, IOWorkLoop * workLoop );
    errno_t bpfAttach( UInt32 dataLinkType, UInt32 headerLength );
    errno_t bpfAttachEN10MB( UInt32 headerLength );
    
    void bpfTapInput(mbuf_t packet, UInt32 dlt, void * header, size_t header_len);
    errno_t bpfTap(u_int32_t data_link_type, bpf_tap_mode direction);
    void sendToBpfTap(mbuf_t packet, UInt32 dlt, void * header, size_t header_len);
    
    ifnet_t getIfnet();
    bool attachIfnet(ether_addr_t * address, char const * bsdName);
    errno_t detachIfnet();
    virtual errno_t configureIfnet();
    errno_t peerToPeerConfigureIfnet();
    
    bool updateLinkParameters( apple80211_interface_availability * params );
    bool updateInterfaceCoexRiskPct( UInt64 percent );
    
    IOLock * getDetachLock();
    void setWaitingForDetach( bool waiting );
    
    void setDebugFlags( UInt64 debugFlags, IOOptionBits options );
    UInt64 debugFlags();
    
    bool shouldLog( UInt64 debugFlags );
    void logDebug( char const * format, ... );
    void logDebug( UInt64 debugFlags, const char * format, ... );
    void vlogDebug( UInt64 debugFlags, char const * format, va_list va );
    void vlogDebugBPF( UInt64 debugFlags, char const * format, va_list va );
    void logTxPacket( mbuf_t packet );
    void logTxCompletionPacket( mbuf_t packet, IOReturn status );
    
    IOReturn storeProcessNameAndIoctlInformation( unsigned long ioctlInfo );
    IOReturn storeIoctlInArray( OSArray * array, OSNumber * ioctlInfo );
    IOReturn dumpUnentitledProcesses();
    
    void handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *); // unimplemented
    bool isAwdlAssistedDiscoveryEnabled();
    void notifyHostapState(apple80211_hostap_state *);
    
    bool dualBandCapable();
    
    // RSN Supplicant
    
    bool initSupplicant( UInt8 * ie, int mode );
    void terminateSupplicant();
    void resetSupplicant();
    bool setPMK( UInt8 * pmk );
    bool setPSKPMK( UInt8 * pmk );
    bool supplicantExchangeComplete();
    bool supplicantInitialized();
    void cachePMKSA( UInt8 * pmk, size_t pmkLen, ether_addr * aa, UInt8 * pmkID );
    SInt32 cachePMKSA( UInt8 * pmk, size_t pmkLen, ether_addr * authenticatorEA );
    void getPMKSAList( struct apple80211_pmk_cache_data * list );

    void getWmeTxCounters( UInt64 * counters );
    void setWoWEnabled(bool enable);
    
    bool controllerLostPower();
    void pushPacket(mbuf_t packet);
    UInt32 printDataPath(userPrintCtx *);
    
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  0 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  1 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  2 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  3 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  4 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  5 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  6 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  7 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  8 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface,  9 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 10 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 11 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 12 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 13 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 14 );
    OSMetaClassDeclareReservedUnused( IO80211VirtualInterface, 15 );

protected:
    UInt16 _dataQueueDepth; //136
    IO80211FlowQueueDatabase * _queueDatabase; //144
    UInt32 _outputSchedulingModel; //152
    UInt32 _outputQueueMaxLength; //156
    IO80211AsyncEventUserClient * _asyncEventUserClient; //160
    IO80211P2PDaemonUserClient * _p2pDaemonUserClient; //168
    UInt64 _debugFlags; //176
    IO80211PeerManager * _peerManager; //184
    IO80211InterfaceMonitor * _interfaceMonitor; //192
    UInt32 _interfaceRole; //200
    IO80211Controller * _controller; //208
    ifnet_t _ifnet; //216
    const char _bsdName[ 16 ]; //224
    UInt32 _ifID; //240
    IOLock * _detachLock; //248
    bool _waitingForDetach; //256
    IO80211LinkState _linkState; //260
    UInt32 _testedWlanPanicPostMessage; //264
    UInt64 _interfaceClosePercent; //272
    UInt64 _interfaceCoexRiskPct; //280
    unsigned long _authTimeout; //288
    IOGatedOutputQueue * _beQueue; //296
    IOGatedOutputQueue * _bgQueue; //304
    IOGatedOutputQueue * _voQueue; //312
    IOGatedOutputQueue * _viQueue; //320
    bool _supportOutputQueues; //328, bg, vo, and vi
    void * _caps; //336, size is 12
    bpfListHeader * _bpfListHead ; //344
    IOLock * _bpfLock; //352
    bool _bpfDLTIsEN10MB; //360
    bpf_tap_mode _tapDirection; //364
    bool _controllerLostPower; //368
    bool _enabledBySystem; //369
    UInt32 _outputFlowControlled; //372
    
    UInt32 _interfaceCCAs[ APPLE80211_MAX_AWDL_CHANNELS ]; // 376
    UInt64 _reserved; // 504
    
    OSDictionary * _deniedProcessesArray; // 512
    bool _wlanEntitlement; // 520
    
    // Interface Namer
    UInt32 _unitNumber; // 524
    const char _unitName [ IFNAMSIZ ]; // 528

    UInt64 __unknown; // 544
    ifmultiaddr_t _ifmaddr; // 552
};

#endif
