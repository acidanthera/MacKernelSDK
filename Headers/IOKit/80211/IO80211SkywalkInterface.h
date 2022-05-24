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

#ifndef _IO80211SKYWALKINTERFACE_H
#define _IO80211SKYWALKINTERFACE_H

#include <IOKit/skywalk/IOSkywalkEthernetInterface.h>
#include <IOKit/80211/IO80211Interface.h>

class IOSkywalkNetworkPacket;
class IO80211Peer;
class IO80211PeerMonitor;
class IO80211LinkQualityMonitor;
class IO80211LinkRecovery;

/*! @defined kInterfaceEnableProperty
    @abstract kInterfaceEnableProperty is the name of the
        Interface Enable property that marks whether the interface
        is functional. */

#define kInterfaceEnableProperty "InterfaceEnable"

/*! @defined kInterfaceBSSIDProperty
    @abstract kInterfaceBSSIDProperty is the IORegistry property
        that records the interface's BSSID. */

#define kInterfaceBSSIDProperty  "InterfaceBSSID"

/*! @defined kInterfacePeerMacProperty
    @abstract kInterfacePeerMacProperty is IORegistry property
        that stores the peer MAC address. */

#define kInterfacePeerMacProperty "InterfacePeerMac"

/*! @defined kIO80211InterfaceRole
    @abstract kIO80211InterfaceRole is the IORegistry property
        that records the role of the skywalk interface. */

#define kIO80211InterfaceRole "IO80211InterfaceRole"

/*! @class IO80211SkywalkInterface
    @abstract Abstract skywalk 80211 interface.
    @discussion ???.
*/

class IO80211SkywalkInterface : public IOSkywalkEthernetInterface
{
    OSDeclareAbstractStructors( IO80211SkywalkInterface )
    
public:
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    UInt32 getInterfaceRole();
    bool setInterfaceRole( UInt32 role );
    const char * getInterfaceRoleStr();
    
    IOReturn createIOReporters( IOService * provider );
    virtual IOReturn configureReport( IOReportChannelList * channels, IOReportConfigureAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport( IOReportChannelList * channels, IOReportUpdateAction action, void * result, void * destination ) APPLE_KEXT_OVERRIDE;
    
    virtual const char * stringFromReturn( IOReturn rtn ) APPLE_KEXT_OVERRIDE;
    
    const char * getBSDName() APPLE_KEXT_OVERRIDE;
    ifnet_t getIfnet();
    IOWorkLoop * getControllerWorkLoop();
    
    virtual IOReturn initBSDInterfaceParameters( struct ifnet_init_eparams * params, sockaddr_dl ** ll ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn prepareBSDInterface( ifnet_t interface, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    virtual errno_t processBSDCommand( ifnet_t interface, UInt32 cmd, void * data ) APPLE_KEXT_OVERRIDE;
    errno_t performGatedCommand( UInt32 cmd, void * data );
    virtual void setRunningState( bool state ) APPLE_KEXT_OVERRIDE;
    
    IOReturn storeProcessNameAndIoctlInformation( unsigned long ioctlInfo );
    IOReturn storeIoctlInArray( OSArray * array, OSNumber * ioctlInfo );
    IOReturn dumpUnentitledProcesses();
    
    virtual void postMessage( UInt32 msg, void * data = NULL, size_t dataSize = 0 );
    virtual bool reportDataPathEvents( UInt32 msg ,void * data = NULL, size_t dataSize = 0 );
    virtual void recordOutputPacket( mbuf_traffic_class_t trafficClass, int txPackets, int txBytes );
    virtual void recordInputPacket( int rxPackets, int rxBytes );
    
    virtual void logTxPacket( IOSkywalkNetworkPacket * packet, struct PacketSkywalkScratch * scratch, mbuf_traffic_class_t trafficClass, bool );
    virtual void logTxCompletionPacket( IOSkywalkNetworkPacket * packet, struct PacketSkywalkScratch * scratch, mbuf_traffic_class_t trafficClass, IOReturn status, UInt32, bool );
    
    virtual bool inputPacket( IOSkywalkNetworkPacket * packet, struct packet_info_tag * info, ether_header * header );
    virtual UInt32 pendingPackets( UInt8 interface );
    virtual UInt32 packetSpace( UInt8 interface );
    
    virtual bool setLinkState( IO80211LinkState linkState, UInt32 reason, bool debounce, UInt32 );
    virtual IO80211LinkState linkState();
    
    virtual void postPeerPresence( ether_addr * address, int32_t rssi, int linkQualityMetric, int nodeProximityMetric, char * serviceInfo );
    virtual void postPeerAbsence( ether_addr * address );
    
    IO80211PeerMonitor * getPeerMonitor( IO80211Peer * peer );
    IO80211LinkQualityMonitor * getLinkQualityMonitor( IO80211Peer * peer );
    IO80211LinkRecovery * getLinkRecovery( IO80211Peer * peer );
    
    virtual void setScanningState( UInt32 scanSource, bool scan, apple80211_scan_data * data, IOReturn status );
    virtual void setDataPathState( bool state );
    
    virtual bool updateLinkParameters( apple80211_interface_availability * params );
    virtual bool updateInterfaceCoexRiskPct( UInt64 percent );
    
    virtual bool setLQM( UInt64 lqm );
    virtual IOReturn updateLinkStatus();
    virtual IOReturn updateLinkStatusGated();
    
    virtual bool setInterfaceExtendedCCA( apple80211_channel channel, apple80211_cca_report * report );
    virtual bool setInterfaceCCA( apple80211_channel channel, int cca );
    virtual bool setInterfaceNF( apple80211_channel channel, long long noiseFloor );
    virtual bool setInterfaceOFDMDesense( apple80211_channel channel, long long ofdmDesense );
    
    virtual void removePacketQueue( const IO80211FlowQueueHash * hash );
    virtual void setDebugFlags( UInt64 debugFlags, IOOptionBits options );
    virtual UInt64 debugFlags();
    
    virtual bool setInterfaceChipCounters( struct apple80211_stat_report * report, apple80211_chip_counters_tx * tx, apple80211_chip_error_counters_tx * err, apple80211_chip_counters_rx * rx );
    virtual bool setInterfaceMIBdot11( apple80211_stat_report * report, apple80211_ManagementInformationBasedot11_counters * counters );
    virtual bool setFrameStats( apple80211_stat_report * frame, apple80211_frame_counters * counters );
    
    virtual void getWmeTxCounters( UInt64 * counters );
    virtual void setEnabledBySystem( bool systemEnabled );
    virtual bool enabledBySystem();
    virtual void willRoam( ether_addr * bssid, UInt32 options );
    virtual void setPeerManagerLogFlag( UInt32 operation, UInt32 bit, IOOptionBits options );
    virtual void setWoWEnabled( bool enable );
    virtual bool wowEnabled();
    
    virtual bool shouldLog( UInt64 debugFlags );
    virtual void logDebug( char const * format, ... );
    virtual void logDebug( UInt64 debugFlags, char const * format, ... );
    virtual void logDebugHex( const void * data, size_t size, const char * format, ... );
    virtual void vlogDebug( UInt64 debugFlags, char const * format, va_list va );
    virtual void vlogDebugBPF( UInt64 debugFlags, char const * format, va_list va );
    
    virtual IOReturn createLinkQualityMonitor( IO80211Peer * peer, IOService * service );
    virtual void releaseLinkQualityMonitor( IO80211Peer * peer );
    virtual void setNotificationProperty( const OSSymbol * name, const OSObject * data );
    virtual void getWorkerMatchingDict( OSString * name );
    
    virtual bool init( IOService * controller );
    virtual bool isInterfaceEnabled();
    
    virtual ether_addr getSelfMacAddr();
    virtual void setSelfMacAddr( ether_addr * address );
    virtual ether_addr getBSSID();
    virtual IOSkywalkPacketBufferPool * getPacketPool( OSString * name );
    virtual void * getLogger();
    
    virtual errno_t handleSIOCSIFADDR();
    errno_t handleDebugCmd( apple80211_debug_command * cmd );
    virtual errno_t debugHandler( apple80211_debug_command * command );
    
    UInt32 printDataPath( userPrintCtx * context );
    UInt32 getDataQueueDepth();
    
public:
    IO80211PeerManager * _peerManager; // 208
    IO80211Controller * _controller; // 216
    IO80211InterfaceMonitor * _interfaceMonitor; // 224
    IO80211AssociationJoinSnapshot * _assocJoinSnapshot; // 232
    UInt32 _interfaceRole; // 240
    UInt32 _interfaceID; // 244
    bool _wlanEntitlement; // 248
    char _bsdName[IFNAMSIZ]; // 249
    OSDictionary * _deniedProcessesArray; // 272
    // 280
    
    OSSymbol * _interfacePeerMacProperty; // 320
    OSSymbol * _interfaceBSSIDProperty;   // 328
    OSSymbol * _interfaceEnableProperty;  // 336
    OSData * _bssidData; // 344
    // 352
    OSDictionary * _interfaceMacAddress; // 360
    OSBoolean * _interfaceEnabled; // 376

    void * _logger; // 384

    bool _runningState; // 408
    IO80211LinkState _linkState; // 412
    ether_addr _selfMacAddress; // 416
    // 424

};

// 728

#endif
