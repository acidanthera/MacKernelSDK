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

#ifndef _IO80211CONTROLLER_H
#define _IO80211CONTROLLER_H

#ifndef __PRIVATE_SPI__
#define __PRIVATE_SPI__
#endif

#include <Availability.h>
#include <libkern/version.h>

#include <sys/kpi_mbuf.h>
#include <sys/param.h>
#include <net/bpf.h>

#include <IOKit/network/IOEthernetController.h>
#include <IOKit/80211/apple80211_ioctl.h>
#include <IOKit/80211/IO80211Interface.h>
#include <IOKit/80211/IO80211FlowQueueDatabase.h>
#include <IOKit/80211/IO80211WorkLoop.h>

#define AUTH_TIMEOUT 15 // seconds

/*! @enum linkSpeed.
     @abstract The link speed of different 80211 protocols.
     @constant LINK_SPEED_80211A 54 Mbps
     @constant LINK_SPEED_80211B 11 Mbps.
     @constant LINK_SPEED_80211G 54 Mbps.
     @constant LINK_SPEED_80211N 300 Mbps.
 */

enum linkSpeed
{
    LINK_SPEED_80211A    = 54000000ul,        // 54 Mbps
    LINK_SPEED_80211B    = 11000000ul,        // 11 Mbps
    LINK_SPEED_80211G    = 54000000ul,        // 54 Mbps
    LINK_SPEED_80211N    = 300000000ul,       // 300 Mbps (MCS index 15, 400ns GI, 40 MHz channel)
};

enum scanSource
{
    kScanSourceScanManagerFamily      = 0x00,
    kScanSourceDriverInterleavedScan  = 0x01,
    kScanSourceDriverScanDone         = 0x02,
    kScanSourceDriverInternalScanDone = 0x03,
    kScanSourceDebugCmd               = 0x04,
    kScanSourceJoining                = 0x05,
    kScanSourceFirmwareStartedScan    = 0x06,
    kScanSourceScanRefused            = 0x07,
    kScanSourceScanError              = 0x08,
    kScanSourceScanBlocked            = 0x09,
    kScanSourceScanDone               = 0x0A,
    kScanSourcesUnknown
};

enum joinStatus {};
enum CCStreamLogLevel {};

enum IO80211CountryCodeOp
{
    kIO80211CountryCodeReset,                // Reset country code to world wide default, and start
    // searching for 802.11d beacon
};

enum IO80211SystemPowerState
{
    kIO80211SystemPowerStateUnknown,
    kIO80211SystemPowerStateAwake,
    kIO80211SystemPowerStateSleeping
};

enum IO80211FeatureCode
{
    kIO80211Feature80211n  = 1,
    kIO80211Feature80211ac = 2,
    kIO80211Feature80211ax = 3
};

class IOSkywalkInterface;
class IO80211Interface;
class IO80211VirtualInterface;
class IO80211SkywalkInterface;
class IO80211InfraInterface;
class IO80211ControllerMonitor;
class IO80211ScanManager;
class IO80211RangingManager;
class IO80211FlowQueue;
class IOReporter;

class CCPipe;
class CCLogPipe;
class CCIOReporterLogStream;
class CCLogStream;
class CCFaultReporter;
class CCDataStream;

struct apple80211_hostap_state;
struct apple80211_channel_switch_announcement;
struct apple80211_beacon_period_data;
struct apple80211_power_debug_sub_info;
struct apple80211_stat_report; // size = 40
struct apple80211_frame_counters;
struct apple80211_leaky_ap_event;
struct apple80211_chip_stats;
struct apple80211_extended_stats;
struct apple80211_ampdu_stat_report;
struct apple80211_btCoex_report;
struct apple80211_cca_report;
struct apple80211_lteCoex_report;

typedef errno_t (*apple80211_ioctl_func)( IO80211Controller * controller, IO80211Interface * interface, IO80211VirtualInterface * virtualInterface, apple80211req * request, bool isNewRequest );
extern apple80211_ioctl_func gGetHandlerTable[];
extern apple80211_ioctl_func gSetHandlerTable[];

class IO80211Controller : public IOEthernetController
{
    OSDeclareAbstractStructors( IO80211Controller )

public:
    virtual bool init( OSDictionary * dictionary = NULL ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
#if __MAC_OS_X_VERSION_MIN_REQUIRED <= MAC_OS_X_VERSION_10_15
    virtual bool terminate( IOOptionBits options ) APPLE_KEXT_OVERRIDE;
#endif

    IOReturn createIOReporters( IOService * provider );
    void releaseIOReporters();
    IOReturn findAndAttachToFaultReporter();
    void resetIO80211ReporterHistory();
    virtual IOReturn configureReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    IOReturn addReporterLegend( IOService * reportLegend, IOReporter * reporter, const char * groupName, const char * subGroupName );
    IOReturn removeReporterFromLegend( IOService * reportLegend, IOReporter * reporter, const char * groupName, const char * subGroupName );
    void lockIOReporterLegend();
    void unlockIOReporterLegend();
    void handleIOReporterTimer( IOTimerEventSource * timer );
    IOReturn logIOReportLogStreamSubscription( UInt64 subscription );
    IOReturn addIOReportLogStreamForProvider( IOService * provider, UInt64 * subscription );
    IOReturn addSubscriptionForThisReporterFetchedOnTimer( IOReporter * reporter, const char * groupName, const char * subGroupName, IOService * driver );
    IOReturn addSubscriptionForProviderFetchedOnTimer( IOService * provider );
    void setIOReportersStreamFlags( UInt64 flags );
    void setIOReportersStreamLevel();
    void updateIOReportersStreamFrequency();

    virtual const char * stringFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;
    virtual int errnoFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;

    errno_t getASSOCIATE_RESULT( IO80211Interface * interface, IO80211VirtualInterface * vif, IO80211SkywalkInterface * skywalk, apple80211_assoc_result_data * result );
    errno_t getASSOCIATE_EXTENDED_RESULT( IO80211Interface * interface, IO80211VirtualInterface * vif, IO80211InfraInterface * infra, apple80211_assoc_result_data * result );
    bool getSSIDData( apple80211_ssid_data * ssid );
    bool getBSSIDData( OSObject * interface, apple80211_bssid_data * bssid );
    bool getBeaconPeriod( apple80211_beacon_period_data * period );
    bool getChipCounterStats( apple80211_chip_stats * chip );
    bool getInfraExtendedStats( apple80211_extended_stats * infra );
    bool getInfraChannel( apple80211_channel_data * channel );
    bool getCountryCode( apple80211_country_code_data * countryCode );

    bool setChanCCA( apple80211_stat_report * stat, int cca );
    bool setChanExtendedCCA( apple80211_stat_report * stat, apple80211_cca_report * cca );
    bool setChanNoiseFloor( apple80211_stat_report * stat, int value );
    bool setChanNoiseFloorLTE( apple80211_stat_report * stat, int value );
    bool setAMPDUstat( apple80211_stat_report * stat, apple80211_ampdu_stat_report * ampdu, apple80211_channel * channel );
    bool setBTCoexstat( apple80211_stat_report * stat, apple80211_btCoex_report * btCoex );
    bool setLTECoexstat( apple80211_stat_report * stat, apple80211_lteCoex_report * lteCoex );
    bool setPowerStats( apple80211_stat_report * stat, apple80211_power_debug_sub_info * info );
    bool setFrameStats( apple80211_stat_report * stat, apple80211_frame_counters * counters, apple80211_channel * channel );
    bool setLeakyAPStats( apple80211_leaky_ap_event * event );
    bool setChipCounterStats( apple80211_stat_report * stat, apple80211_chip_stats * chip, apple80211_channel * channel );
    bool setExtendedChipCounterStats( apple80211_stat_report * stat, void * chip );
    errno_t setCountryCode( apple80211_country_code_data * countryCode );

    void log11axAsrTxLatency( UInt64 frequency, UInt32 channel );
    IOReturn get11axAsrTxLatencyClearOnRead( OSData * data );

    bool forceInterfaceRegistration( IO80211Interface * interface );
    bool attachInterfaceWithMacAddress( void * macAddress, UInt32 addrLen, IONetworkInterface ** outInterface, bool doRegister, UInt32 attachTimeout );

    virtual bool attachInterface( IONetworkInterface ** interface, bool doRegister = true ) APPLE_KEXT_OVERRIDE;
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual void detachInterface( IONetworkInterface * interface, bool sync = false ) APPLE_KEXT_OVERRIDE;
#endif
    virtual IONetworkInterface * createInterface() APPLE_KEXT_OVERRIDE;
    virtual bool configureInterface( IONetworkInterface * interface ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn outputStart( IONetworkInterface * interface, IOOptionBits options ) APPLE_KEXT_OVERRIDE;

    virtual void requestPacketTx( void * packet, UInt32 size );
    virtual IOReturn getHardwareAddress( IOEthernetAddress * addrP ) APPLE_KEXT_OVERRIDE { return kIOReturnUnsupported; }
    virtual IOReturn getHardwareAddressForInterface( IO80211Interface * interface, IOEthernetAddress * addrP );
    virtual void inputMonitorPacket( mbuf_t packet, UInt32 dataLinkType, void * header, size_t headerSize );
    virtual errno_t outputRaw80211Packet( IO80211Interface * interface, mbuf_t packet );
    virtual errno_t outputActionFrame( IO80211Interface * interface, mbuf_t packet );
    virtual errno_t bpfOutputPacket( OSObject *, UInt32 dataLinkType, mbuf_t packet );
    virtual errno_t monitorModeSetEnabled( IO80211Interface * interface, bool enabled, IOOptionBits options ) { return ENXIO; }
    void setDisplayState( bool state );

    virtual IOService * getProvider() const APPLE_KEXT_OVERRIDE;
    virtual bool createWorkLoop() APPLE_KEXT_OVERRIDE;
    virtual IOWorkLoop * getWorkLoop() const APPLE_KEXT_OVERRIDE;
    virtual IOOutputQueue * getOutputQueue() const APPLE_KEXT_OVERRIDE;
    virtual IO80211Interface * getNetworkInterface();
    IO80211ScanManager * getPrimaryInterfaceScanManager();
    IO80211LinkState getPrimaryInterfaceLinkState();
    void setPrimaryInterfaceDatapathState( bool state );
    IO80211SkywalkInterface * getInfraInterface();
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual IO80211SkywalkInterface * getPrimarySkywalkInterface();
#endif
    IO80211ControllerMonitor * getInterfaceMonitor();

    virtual errno_t apple80211_ioctl( IO80211Interface * interface, IO80211VirtualInterface * virtualInterface, ifnet_t netif, unsigned long cmd, void * data );
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual errno_t apple80211_ioctl( IO80211SkywalkInterface * interface, unsigned long cmd, void * data );
#endif
    virtual errno_t apple80211_ioctl( IO80211Interface * interface, ifnet_t netif, unsigned long cmd, void * data );
    virtual errno_t apple80211Request( UInt32 req, int type, IO80211Interface * interface, void * data );
    errno_t apple80211RequestIoctl( UInt32 req, int type, IO80211Interface * interface, void * data );
    errno_t apple80211VirtualRequestIoctl( UInt32 req, int type, IO80211VirtualInterface * interface, void * data );
    virtual errno_t apple80211VirtualRequest( UInt32 req, int type, IO80211VirtualInterface * virtualInterface, void * data ) { return EOPNOTSUPP; }
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual errno_t apple80211SkywalkRequest( UInt32 req, int type, IO80211SkywalkInterface * skywalkInterface, void * data ) { return EOPNOTSUPP; }
#endif

    virtual errno_t stopDMA() { return EOPNOTSUPP; }
    virtual UInt32 hardwareOutputQueueDepth( IO80211Interface * interface ) { return 0; }
    virtual errno_t performCountryCodeOperation( IO80211Interface * interface, IO80211CountryCodeOp operation ) { return EOPNOTSUPP; }
    virtual bool useAppleRSNSupplicant( IO80211Interface * interface );
    virtual bool useAppleRSNSupplicant( IO80211VirtualInterface * virtualInterface );
    virtual void dataLinkLayerAttachComplete( IO80211Interface * interface );

    void enableFeatureForLoggingFlags( UInt64 flags ) {}
    virtual errno_t enableFeature( IO80211FeatureCode feature, void * data ) { return EOPNOTSUPP; }
    virtual errno_t setVirtualHardwareAddress( IO80211VirtualInterface * interface, ether_addr * address ) { return EOPNOTSUPP; }
    virtual IOReturn enableVirtualInterface( IO80211VirtualInterface * interface ) { return kIOReturnSuccess; }
    virtual IOReturn disableVirtualInterface( IO80211VirtualInterface * interface ) { return kIOReturnSuccess; }
    virtual bool requiresExplicitMBufRelease() { return false; }
    virtual bool flowIdSupported() { return false; }
    virtual IO80211FlowQueueLegacy * requestFlowQueue( const struct FlowIdMetadata * );
    virtual void releaseFlowQueue( IO80211FlowQueue * queue );
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual void getLogPipes( CCPipe **, CCPipe **, CCPipe ** ) {}
#endif

    IOReturn requestQueueSizeAndTimeout( UInt16 * size, UInt16 * timeout ) { return kIOReturnUnsupported; }
    virtual IOReturn enablePacketTimestamping() { return kIOReturnUnsupported; }
    virtual IOReturn disablePacketTimestamping() { return kIOReturnUnsupported; }
    virtual IOReturn selfDiagnosticsReport( IOReturn error, const char * errorString, IOOptionBits options );
    virtual UInt32 getDataQueueDepth( OSObject * queue ) { return 1024; }
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_11_0
    virtual bool isAssociatedToMovingNetwork() { return false; }
#endif

    virtual mbuf_flags_t inputPacket( mbuf_t packet );
    void dispatchPacket( mbuf_t packet, UInt32, char * name, UInt32 );

    virtual errno_t apple80211_ioctl_get( IO80211Interface * interface, IO80211VirtualInterface * virtualInterface, ifnet_t netif, void * data );
    virtual errno_t apple80211_ioctl_get( IO80211SkywalkInterface * skywalk, void * data );
    errno_t apple80211_ioctl_get( IO80211Interface * interface, IO80211VirtualInterface * vif, IO80211SkywalkInterface * skywalk, void * data );

    virtual errno_t apple80211_ioctl_set( IO80211Interface * interface, IO80211VirtualInterface * virtualInterface, ifnet_t netif, void * data );
    virtual errno_t apple80211_ioctl_set( IO80211SkywalkInterface * skywalk, void * data );
    errno_t apple80211_ioctl_set( IO80211Interface * interface, IO80211VirtualInterface * vif, IO80211SkywalkInterface * skywalk, void * data );

    virtual bool attachInterface( IOSkywalkInterface * skywalkInterface, IOService * provider );

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_11_0
    virtual bool detachInterface( IOSkywalkInterface * skywalkInterface, bool doRegister );
#endif
    virtual IO80211VirtualInterface * createVirtualInterface( ether_addr * address, UInt32 role );
    virtual bool attachVirtualInterface( IO80211VirtualInterface ** interface, ether_addr * address, UInt32 role, bool doRegister );
    virtual bool detachVirtualInterface( IO80211VirtualInterface * interface, bool doRegister );

    virtual IOReturn enable( IONetworkInterface * interface ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn disable( IONetworkInterface * interface ) APPLE_KEXT_OVERRIDE;
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
    virtual IOReturn enable( IO80211SkywalkInterface * skywalkInterface );
    virtual IOReturn disable( IO80211SkywalkInterface * skywalkInterface );
#endif

    UInt32 assignUnitNumber( const char * name );
    void markInterfaceUnitUsed( const char * name, UInt32 number );
    void markInterfaceUnitUnused( const char * name, UInt32 number );

    void configureAntennae();
    UInt32 radioCountForInterface( IO80211Interface * interface );

    void setCurrentChannel( apple80211_channel * channel );
    void setInfraChannel( apple80211_channel * channel );

    void setHtCapability( ieee80211_ht_capability_ie * capability );
    struct ieee80211_ht_capability_ie * getHtCapability();
    UInt32 getHtCapabilityLength();

    void logDebug( char const * format, ... );
    void logDebug( UInt64 flags, char const * format, ... );
    void vlogDebug( UInt64 flags, char const * format, va_list args );
    void vlogDebugBPF( UInt64 flags, char const * format, va_list args ) {}
    void clearLogTargets();
    void setLogTarget( IO80211Interface * interface, IO80211VirtualInterface * vif, bool mode );
    bool io80211isDebuggable( bool * enable );
    IOReturn setupControlPathLogging();
    bool isIOCTLLoggingRestricted( apple80211req * request );
    void logIOCTL( apple80211req * request );

    void setChannelSequenceList( apple80211_awdl_sync_channel_sequence * sequence );
    void calculateInterfacesAvaiability();
    void calculateInterfacesCoex();
    bool updateInterfaceCoexRiskPct( UInt64 percent );
    void printChannels();
    void scanStarted( scanSource source, apple80211_scan_data * data );
    void scanDone( scanSource source, int );
    void handleChannelSwitchAnnouncement( apple80211_channel_switch_announcement * announcement );
    void joinStarted( scanSource source, joinStatus status );
    void joinDone( scanSource source, joinStatus status );
    bool isAwdlAssistedDiscoveryEnabled();
    void notifyHostapState( apple80211_hostap_state * state );
    bool inputInfraPacket( mbuf_t packet );

    int copyIn( user_addr_t uaddr, void * kaddr, size_t len );
    int copyOut( const void * kaddr, user_addr_t udaddr, size_t len );

protected:
    static IOReturn powerChangeHandler( void * target, void * refCon, UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize );
    static IOReturn powerChangeGated( OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3 );

    OSMetaClassDeclareReservedUnused( IO80211Controller,  0 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  1 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  2 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  3 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  4 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  5 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  6 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  7 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  8 );
    OSMetaClassDeclareReservedUnused( IO80211Controller,  9 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 10 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 11 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 12 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 13 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 14 );
    OSMetaClassDeclareReservedUnused( IO80211Controller, 15 );

protected:
    IOTimerEventSource * _fReporterGatheringTimer; //280
    OSArray * _fReporterSubscriptions; //288
    UInt32 _fReporterStreamFrequency; //296
    IOSkywalkInterface * _fSkywalkInterface; //304
    bool _isDebugSetUp; //312
    UInt32 _debugValue; //316
    IORecursiveLock * _fIOReportLegendLock; //320

    ieee80211_ht_capability_ie _htCapability; //328
    UInt32 _htCapabilityLength; //356

    IO80211ControllerMonitor * _fControllerMonitor; //360

    CCLogPipe * _fControllerIOReporterPipe; //368
    CCIOReporterLogStream * _fControllerIOReporterStream; //376
    uint64_t __reserved0; //384
    CCLogPipe * _controlPathLogPipe; //392
    CCLogStream * _ioctlLogStream; //400
    CCLogStream * _eventLogStream; //408
    CCPipe * _debugDataPipe; //416
    CCDataStream * _faultReportDataStream; //424
    CCFaultReporter * _faultReporter; //432

    UInt32 _11axAsrTxLatency[16]; //440

    bool _ifAttachPending; // 504
    IO80211WorkLoop * _fWorkLoop; //512
    IONetworkInterface * _netIF; //520
    IO80211VirtualInterface * _primaryVIF; //528
    IO80211VirtualInterface * _secondaryVIF; // 536
    uint64_t __reserved1; // 544
    IO80211VirtualInterface * _vifLogTargets[4]; // 552
    IO80211Interface * _netIFLogTarget; //584

    apple80211_channel _infraChannel; //592
    apple80211_channel _currentChannel; //604
    apple80211_awdl_sync_channel_sequence _channelSequenceList; //616

    IONotifier * _fPowerChangeNotifier; //1016
    IOService * _fProvider; //1024
    IO80211RangingManager * _fRangingManager; //1032
    bool _wowEnabled; //1040
    IONotifier * _fEFINVRAMPublishedNotifier; //1048
    UInt8 _io8LogReferenceCount; //1056
    void * _refCon; //1064
};

#endif
