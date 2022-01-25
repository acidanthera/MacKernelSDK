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

#ifndef _IO80211INTERFACE_H
#define _IO80211INTERFACE_H

#include <Availability.h>
#include <libkern/version.h>

#include <net/if_var.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/80211/apple80211_ioctl.h>
#include <IOKit/80211/IO80211FlowQueueDatabase.h>
#include <IOKit/80211/IO80211WorkLoop.h>

typedef enum IO80211LinkState
{
    kIO80211NetworkLinkUndefined,           // Starting link state when an interface is created
    kIO80211NetworkLinkDown,                // Interface not capable of transmitting packets
    kIO80211NetworkLinkUp                   // Interface capable of transmitting packets
} IO80211LinkState;

typedef enum kIO80211InterfaceType
{
    kIO80211InterfaceTypeInfra,
    kIO80211InterfaceTypeAPSta,
    kIO80211InterfaceTypeAWDL,
    kIO80211InterfaceTypeNAN
} kIO80211InterfaceType;

struct io80211_timespec
{
    clock_sec_t  secs;
    clock_nsec_t nanosecs;
};

struct apple80211_interface_availability
{
    UInt64 fExpectedPeakLatency;
    UInt64 x;
    UInt64 y;
    // x/y = open percentage...
};

/*! @defined kIO80211InterfaceClass
    @abstract The name of the IO80211Interface class.
*/
#define kIO80211InterfaceClass     "IO80211Interface"

class RSNSupplicant;
class IO80211Controller;
class IO80211InterfaceMonitor;
class IO80211ScanManager;
class IO80211PeerManager;
class IO80211AssociationJoinSnapshot;
class IO80211AsyncEventUserClient;
class CCFaultReporter;

struct packet_info_tx;
struct apple80211_debug_command;
struct apple80211_chip_error_counters_tx;
struct apple80211_chip_counters_tx;
struct apple80211_chip_counters_rx;

extern const char * ssidBufToString(UInt8 * data, unsigned long dataSize, char * string);
extern clock_nsec_t timespecSub(io80211_timespec * inSpec1, io80211_timespec * inSpec2, io80211_timespec * outSpec);
extern void io80211_get_calendar_time(io80211_timespec * spec);
extern bool procIsSuperuser();
extern UInt32 calculateChannelBandwidth(UInt32);
extern UInt32 channelToFrequency(UInt32, UInt32);
extern bool isETSICountryCode(unsigned char * codes);

typedef errno_t (OSObject::*IOBPFTapAction)(u_int32_t data_link_type, bpf_tap_mode direction);

struct bpfNode
{
    bpfNode * next; // 0
    bpfNode * prev; // 8
    UInt32 dataLinkType; // 16
    IOGatedOutputQueue * outputQueue; // 24
    OSObject * target; // 32
    IOBPFTapAction tap; // 40
    OSObject * tapOwner; // 48
    bpf_tap_mode direction; // 56
};

struct bpfListHeader
{
    bpfNode * first;
};

/*! @class IO80211Interface
     @abstract The 80211 interface object.
     @discussion An 80211 controller driver,
     that is a subclass of IOEthernetInterface, will instantiate an object
     of this class when the driver calls the attachInterface() method.
     This interface object will then vend an Ethernet interface to DLIL,
     and manage the connection between the controller driver and the upper
     networking layers. Drivers will seldom need to subclass
     IO80211Interface.
*/

class IO80211Interface : public IOEthernetInterface
{
    OSDeclareDefaultStructors( IO80211Interface );

public:
    virtual bool init( IONetworkController * controller ) APPLE_KEXT_OVERRIDE;
    virtual bool terminate( IOOptionBits options = 0 ) APPLE_KEXT_OVERRIDE;
    
    virtual bool attach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void detach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    IOReturn finishAttachToDataLinkLayer();
    static IOReturn finishAttachToDataLinkLayerGated( OSObject * target, void * arg0, void * arg1, void * arg2, void * arg3 );
    virtual IOReturn attachToDataLinkLayer( IOOptionBits options, void * parameter ) APPLE_KEXT_OVERRIDE;
    virtual void detachFromDataLinkLayer( IOOptionBits options, void * parameter ) APPLE_KEXT_OVERRIDE;
    
    virtual bool inputEvent( UInt32 type, void * data ) APPLE_KEXT_OVERRIDE;
    
    virtual void setPoweredOnByUser( bool userPowered );
    bool poweredOnByUser();
    virtual void setEnabledBySystem( bool systemEnabled );
    bool enabledBySystem();
    
    IOReturn IO80211InterfacePostMessage( UInt32 msg ,void * data = NULL, size_t dataSize = 0 );
    void postMessage( UInt32 msg, void * data = NULL, size_t dataSize = 0 );
    IOReturn setDataPointerAndLengthForMessageType( apple80211_postMessage_tlv_types type, void ** data, size_t * dataSize );
    
    virtual IOReturn configureReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport( IOReportChannelList * channels, IOReportUpdateAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    IOReturn createIOReporters( IOService * service );
    
    void reportTransmitStatus( mbuf_t packet, int len, struct packet_info_tx * info );
    IOReturn reportTransmitCompletionStatus( mbuf_t packet, int len, UInt32, UInt32, UInt32 );
    IOReturn reportDataPathEvents( UInt32 msg ,void * data = NULL, size_t dataSize = 0 );
    static IOReturn reportDataPathEventsGated( void * target, void * msg, void * data, void * dataSize, void * arg0 );
    IOReturn reportTxStatistics( apple80211_txstats * stats );
    IOReturn reportDataTransferRates();
    static IOReturn reportDataTransferRatesStatic( void * target );
    void reportDataTransferRatesGated();
    
    void setAuthTimeout( AbsoluteTime timeout );
    AbsoluteTime authTimeout();
    
    virtual bool setLinkState( IO80211LinkState, UInt32 reason );
    virtual bool setLinkState( IO80211LinkState, int, UInt32 reason );
    IO80211LinkState linkState();
    
    void setScanningState(UInt32 scanSource, bool, apple80211_scan_data *, int);
    
    bool setInterfaceExtendedCCA( apple80211_channel channel, apple80211_cca_report * report );
    bool setInterfaceCCA( apple80211_channel channel, int cca );
    bool setInterfaceNF( apple80211_channel channel, long long noiseFloor );
    bool setInterfaceOFDMDesense( apple80211_channel channel, long long ofdmDesense );
    bool setInterfaceChipCounters( struct apple80211_stat_report * report, apple80211_chip_counters_tx * tx, apple80211_chip_error_counters_tx * err, apple80211_chip_counters_rx * rx );
    bool setInterfaceMIBdot11( apple80211_stat_report * report, apple80211_ManagementInformationBasedot11_counters * counters );
    bool setFrameStats( apple80211_stat_report * frame, apple80211_frame_counters * counters );
    
    bool setLeakyAPStatsMode( UInt32 mode ); // 0: Disable 1: Enable 2: Unsupported
    bool getLeakyApStats( const struct apple80211_leaky_ap_stats ** stats );
    bool resetLeakyApStats();
    bool setLeakyApSsidMetrics( struct apple80211_leaky_ap_ssid_metrics * metrics );
    bool setLeakyAPStats( struct apple80211_leaky_ap_event * event );
    void handleLeakyApStatsModeTimer( IOTimerEventSource * timer );
    void handleLeakyApStatsResetTimer( IOTimerEventSource * timer );
    
    bool getExtendedStats( struct apple80211_extended_stats * stats );
    
    bool initSupplicant( UInt8 * ie, int mode );
    void terminateSupplicant();
    void resetSupplicant();
    bool setPMK( UInt8 * pmk, UInt8 * pmkID );
    bool setPSKPMK( UInt8 * pmk );
    bool supplicantExchangeComplete();
    bool supplicantInitialized();
    UInt32 outputEAPOLFrame( mbuf_t m );
    void cachePMKSA( UInt8 * pmk, size_t pmkLen, ether_addr * aa, UInt8 * pmkID );
    SInt32 cachePMKSA( UInt8 * pmk, size_t pmkLen, ether_addr * authenticatorEA );
    struct rsn_pmksa_node * pmksaLookup( ether_addr * authenticatorEA, UInt8 * pmkID );
    void getPMKSAList( struct apple80211_pmk_cache_data * list );
    
    bool shouldRoam( struct apple80211_scan_result * asr );
    void willRoam( ether_addr * bssid, UInt32 options );
    
    virtual UInt32 inputPacket( mbuf_t packet, UInt32 length = 0, IOOptionBits options = 0, void * param = 0 ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn outputPacket( mbuf_t m, void * param );
    
    IO80211WorkLoop * getControllerWorkLoop();
    IO80211Controller * getController();
    const char * getBSDName();
    
    void setCountermeasuresTimer( IOTimerEventSource * timer );
    static void stopCountermeasures( OSObject * owner, IOTimerEventSource * sender );
    
    void updateChannelProperty();
    static void updateChannelPropertyStatic( void * owner );
    void updateChannelPropertyGated();
    void updateSSIDProperty();
    void updateBSSIDProperty();
    void updateStaticProperties();
    void updateCountryCodeProperty( bool shouldLog );
    static IOReturn performCountryCodeOpGated( OSObject * owner, void * inInterface, void * inParams, void * arg0, void * arg1 );
    
    IOReturn updateLinkStatus();
    static IOReturn updateLinkStatusStatic( void * owner );
    IOReturn updateLinkStatusGated();
    bool updateLinkSpeed();
    bool updateLinkParameters( apple80211_interface_availability * params );
    bool updateLinkParametersStatic( void * target, void * params );
    bool updateLinkParametersGated( apple80211_interface_availability * params );
    
    bool updateInterfaceCoexRiskPct( UInt64 percent );
    bool setBTCoexWLANLostAntennaTime( UInt64, UInt64, bool, struct apple80211_btCoex_report * report ); // percentages?
    void configureAntennae();
    
    void setDebugFlags( UInt64 debugFlags, IOOptionBits options );
    UInt64 debugFlags();
    
    void setPeerManagerLogFlag( UInt32 operation, UInt32 bit, IOOptionBits options );
    void togglePeerManagerLogFlag( UInt32 bit, IOOptionBits options );
    
    void logDebug( char const * format, ... );
    void logDebug( UInt64 debugFlags, char const * format, ... );
    void logDebugHex( const void * data, size_t size, const char * format, ... );
    void vlogDebug( UInt64 debugFlags, char const * format, va_list va );
    void vlogDebugBPF( UInt64 debugFlags, char const * format, va_list va );
    bool shouldLog( UInt64 debugFlags );
    void logTxCompletionPacket( mbuf_t packet, int len );
    
    virtual const char * stringFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;
    virtual int errnoFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;
    
    IOGatedOutputQueue * getOutputQueue();
    IOGatedOutputQueue * getOutputQueueForDLT( UInt32 dlt );
    void configureBpfOutputQueues(bool);
    void startOutputQueues();
    void stopOutputQueues();
    
    IO80211FlowQueue * findOrCreateFlowQueue( IO80211FlowQueueHash hash );
    IO80211FlowQueue * findExistingFlowQueue(IO80211FlowQueueHash hash);
    void flushPacketQueues();
    void removePacketQueue( const IO80211FlowQueueHash * hash );
    UInt32 pendingPackets( UInt8 interface );
    UInt32 queueSize( UInt8 interface );
    UInt32 packetSpace( UInt8 interface );
    
    errno_t bpfAttach( UInt32 dataLinkType, UInt32 headerLength, OSObject * target, IOOutputAction action, IOBPFTapAction tap, IOWorkLoop * workLoop );
    errno_t bpfAttach( UInt32 dataLinkType, UInt32 headerLength );
    
    void monitorModeInputPacket(mbuf_t packet, UInt32 dlt, void * header, size_t header_len);
    void bpfTapInput(mbuf_t packet, UInt32 dlt, void * header, size_t header_len);
    errno_t bpfTap(u_int32_t data_link_type, bpf_tap_mode direction);
    
    errno_t bpfOutputPacket(mbuf_t, void *);
    errno_t bpfOutput(uint,mbuf_t);
    mbuf_t preQueuePacket(mbuf_t);
    static IOReturn outputPreEnqueueHandler( void * owner , void *, mbuf_t packet );
    void logTxPacket(mbuf_t);
    void dropTxPacket( mbuf_t packet );
    UInt32 dequeueTxPackets(UInt32, UInt32);
    UInt32 dequeueTxPackets(struct TxPacketRequest * request);
    
    virtual errno_t setLinkQualityMetric( int quality );
    bool setLQM( UInt64 lqm );
    bool setLQMStatic( void * owner, void * lqm );
    bool setLQMGated( UInt64 lqm );
    
    IOReturn storeProcessNameAndIoctlInformation( unsigned long ioctlInfo );
    IOReturn storeIoctlInArray( OSArray * array, OSNumber * ioctlInfo );
    IOReturn dumpUnentitledProcesses();
    
    UInt32 queueWMEPacket( mbuf_t m, void * param );
    void getWmeTxCounters( UInt64 * counters );
    
    bool shortGISupported40MHz();
    bool shortGISupported20MHz();
    void setWoWEnabled(bool enable);
    
    IOReturn outputStart(UInt32);
    IOReturn configureInterface();
    void setDataPathState( bool state );
    
    bool setPidLock(bool locked);
    bool pidLocked();
    
    bool netBooting();
    void setNetBooting();
    void netBootThreadGated(OSObject *,void *,void *,void *,void *);
    void netBootThread(IOService *);
    void associateForNetBoot(IOService *);
    static IOReturn associateForNetBootGated(OSObject *,void *,void *,void *,void *);
    static bool efiNVRAMPublished( void * target, void * refCon, IOService * newService, IONotifier * notifier );
    
    struct apple80211_ap_cmp_data * apCompare(apple80211_ap_cmp_data *,apple80211_ap_cmp_data *);
    UInt32 printDataPath( struct userPrintCtx *);
    void printPeers(uint,uint);
    
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual IOReturn newUserClient( task_t owningTask, void * securityID, UInt32 type, OSDictionary * properties, LIBKERN_RETURNS_RETAINED IOUserClient ** handler ) APPLE_KEXT_OVERRIDE;
#endif
    IOReturn resetUserClientReference();
    static IOReturn resetUserClientReferenceGated( OSObject * target, void * arg0, void * arg1, void * arg2, void * arg3 );
    IOReturn startAsyncEventUserClientForTask( task_t task, kIO80211InterfaceType );
    
    OSString * createAssocHistory();
    void clearAssocHistory();
    
protected:
    virtual SInt32 performCommand( IONetworkController * controller, unsigned long cmd, void * arg0, void * arg1 ) APPLE_KEXT_OVERRIDE;
    static SInt32 performGatedCommand( void * target, void * controller, void * cmd, void * arg0, void * arg1 );
    virtual errno_t handleDebugCmd( apple80211_debug_command * cmd );
    virtual void free() APPLE_KEXT_OVERRIDE;
    static IOReturn powerChangeHandler( void * target, void * refCon, UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize );
    
private:
    size_t inputAWSPacket( mbuf_t m );
    void awsRespond( mbuf_t m, struct AWSRequest * requestList, size_t numRequests, UInt16 packetID );
    void purgePMKSACache();
    void freePMKSACache();
    void freeBpf();
    void stopBpf();
    
    OSMetaClassDeclareReservedUnused( IO80211Interface,  0);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  1);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  2);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  3);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  4);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  5);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  6);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  7);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  8);
    OSMetaClassDeclareReservedUnused( IO80211Interface,  9);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 10);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 11);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 12);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 13);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 14);
    OSMetaClassDeclareReservedUnused( IO80211Interface, 15);
    
protected:
    IO80211PeerManager * _peerManager; // 328
    UInt16 _dataQueueDepth; // 336
    IO80211FlowQueueDatabase * _flowQueueDataBse; // 344
    
    UInt64 x; // 352
    UInt64 _linkSpecificInfo; // 360
    UInt32 _linkStatusFlag; // 368
    UInt32 _linkQualityMetric; // 372
    
    UInt32 _effectiveTxBWSinceLastRead; // 376
    UInt64 _effectiveDataTxRate; // 380
    UInt32 yout; // 388
    UInt32 _expectedTxPeakLatency; // 392
    UInt32 _txRetransmitRate; // 396
    UInt32 _totalTxPacketsGivenUp; // 400
    UInt32 _txErrorRate; // 404
    
    UInt32 _effectiveRxBWSinceLastRead; // 408
    UInt64 _effectiveDataRxRate; // 412
    UInt32 yin; // 420
    UInt32 _expectedRxPeakLatency; // 424
    UInt32 _rxErrorRate; // 428
    
    UInt32 _channelFrequency; // 432
    
    IO80211LinkState _linkState; // 520
    
    RSNSupplicant * _rsnSupplicant; // 584
    
    ifmultiaddr_t _awsAddr; // 592
    
    IOTimerEventSource * _countermeasuresTimer; // 600
    
    char _bsdName[IFNAMSIZ]; // 608
    
    UInt64 _debugFlags; // 640
    IOGatedOutputQueue * _outputQueue; // 648
    IO80211Controller * _controller; // 656
    CCFaultReporter * _faultReporter; // 664
    void * _linkLayerAddress; // 672
    UInt8 _interfaceType; // 680
    
    //681
    //682
    //683
    IOGatedOutputQueue * _backgroundOutputQueue; // 688
    IOGatedOutputQueue * _voiceOutputQueue; // 696
    IOGatedOutputQueue * _videoOututQueue; // 704
    
    IO80211ScanManager * _scanManager; // 712
    IO80211InterfaceMonitor * _interfaceMonitor; // 720
    IO80211AssociationJoinSnapshot * _joinSnaposhot; // 728
    bool _wowEnabled; // 736
    int _processID; // 740
    clock_sec_t _pidLockTime; // 744
    thread_call_t _netBootThread; // 752
    bool _netBooting; // 760
    UInt32 _roamThreshold; // 764
    
    // 768
    
    bpfListHeader * _bpfListHead ; // 776
    IOLock * _bpfLock; // 784
    
    UInt64 _reserved; // 792
    
    OSDictionary * _deniedProcessesArray; // 800
    bool _wlanEntitlement; // 808
    bool _terminateCalled; // 809
    
    void * _refCon; // 816, never used...
    
    UInt32 _phyMode; // 824
    bool _awdlLinkDown; // 828
    bool _nanLinkDown; // 829
    
    IO80211AsyncEventUserClient * _userClient; // 832
};

#endif

