//
//  IO80211Controller.h
//  IO80211Family
//

#ifndef _IO80211CONTROLLER_H
#define _IO80211CONTROLLER_H

#if defined(KERNEL) && defined(__cplusplus)

#include <Availability.h>
#include <libkern/version.h>

// This is necessary, because even the latest Xcode does not support properly targeting 11.0.
#ifndef __IO80211_TARGET
#error "Please define __IO80211_TARGET to the requested version"
#endif

#if VERSION_MAJOR > 8
#define _MODERN_BPF
#endif

#include <sys/kpi_mbuf.h>

#include <IOKit/network/IOEthernetController.h>

#include <sys/param.h>
#include <net/bpf.h>

#include "apple80211_ioctl.h"
#include "IO80211SkywalkInterface.h"
#include "IO80211WorkLoop.h"

#define AUTH_TIMEOUT            15    // seconds

/*! @enum LinkSpeed.
 @abstract ???.
 @discussion ???.
 @constant LINK_SPEED_80211A 54 Mbps
 @constant LINK_SPEED_80211B 11 Mbps.
 @constant LINK_SPEED_80211G 54 Mbps.
 */
enum {
    LINK_SPEED_80211A    = 54000000ul,        // 54 Mbps
    LINK_SPEED_80211B    = 11000000ul,        // 11 Mbps
    LINK_SPEED_80211G    = 54000000ul,        // 54 Mbps
    LINK_SPEED_80211N    = 300000000ul,        // 300 Mbps (MCS index 15, 400ns GI, 40 MHz channel)
};

enum IO80211CountryCodeOp
{
    kIO80211CountryCodeReset,                // Reset country code to world wide default, and start
    // searching for 802.11d beacon
};
typedef enum IO80211CountryCodeOp IO80211CountryCodeOp;

enum IO80211SystemPowerState
{
    kIO80211SystemPowerStateUnknown,
    kIO80211SystemPowerStateAwake,
    kIO80211SystemPowerStateSleeping,
};
typedef enum IO80211SystemPowerState IO80211SystemPowerState;

enum IO80211FeatureCode
{
    kIO80211Feature80211n = 1,
};
typedef enum IO80211FeatureCode IO80211FeatureCode;


class IOSkywalkInterface;
class IO80211ScanManager;
enum CCStreamLogLevel
{
    LEVEL_1,
};

enum scanSource
{
    SOURCE_1,
};

enum joinStatus
{
    STATUS_1,
};

class IO80211Controller;
class IO80211Interface;
class IO82110WorkLoop;
class IO80211VirtualInterface;
class IO80211ControllerMonitor;
class CCLogPipe;
class CCIOReporterLogStream;
class CCLogStream;
class IO80211VirtualInterface;
class IO80211RangingManager;
class IO80211FlowQueue;
class IO80211FlowQueueLegacy;
class FlowIdMetadata;
class IOReporter;
extern void IO80211VirtualInterfaceNamerRetain();


struct apple80211_hostap_state;

struct apple80211_awdl_sync_channel_sequence;
struct ieee80211_ht_capability_ie;
struct apple80211_channel_switch_announcement;
struct apple80211_beacon_period_data;
struct apple80211_power_debug_sub_info;
struct apple80211_stat_report;
struct apple80211_frame_counters;
struct apple80211_leaky_ap_event;
struct apple80211_chip_stats;
struct apple80211_extended_stats;
struct apple80211_ampdu_stat_report;
struct apple80211_btCoex_report;
struct apple80211_cca_report;
class CCPipe;
struct apple80211_lteCoex_report;

//typedef int scanSource;
//typedef int joinStatus;
//typedef int CCStreamLogLevel;
typedef IOReturn (*IOCTL_FUNC)(IO80211Controller*, IO80211Interface*, IO80211VirtualInterface*, apple80211req*, bool);
extern IOCTL_FUNC gGetHandlerTable[];
extern IOCTL_FUNC gSetHandlerTable[];

class IO80211Controller : public IOEthernetController {
    OSDeclareAbstractStructors(IO80211Controller)

public:

	virtual void requestPacketTx(void*, unsigned int);

    virtual IOReturn getHardwareAddressForInterface(IO80211Interface *,IOEthernetAddress *);
    virtual void inputMonitorPacket(mbuf_t,unsigned int,void *,unsigned long);
    virtual int outputRaw80211Packet(IO80211Interface *,mbuf_t);

    virtual int outputActionFrame(IO80211Interface *,mbuf_t);

	virtual int bpfOutputPacket(OSObject *,unsigned int,mbuf_t);

	virtual SInt32 monitorModeSetEnabled(IO80211Interface*, bool, unsigned int);

    virtual IO80211Interface* getNetworkInterface(void);

    virtual IO80211SkywalkInterface* getPrimarySkywalkInterface(void);

    virtual SInt32 apple80211_ioctl(IO80211Interface *, IO80211VirtualInterface*, ifnet_t,unsigned long,void *);
    virtual SInt32 apple80211_ioctl(IO80211SkywalkInterface *,unsigned long,void *);

	virtual SInt32 apple80211_ioctl(IO80211Interface *, ifnet_t,unsigned long id,void *) { return 0; };

    virtual SInt32 apple80211Request(unsigned int, int, IO80211Interface*, void*) = 0;

	virtual SInt32 apple80211VirtualRequest(unsigned int,int,IO80211VirtualInterface *,void *);

    virtual SInt32 apple80211SkywalkRequest(unsigned int,int,IO80211SkywalkInterface *,void *);
    virtual SInt32 stopDMA() { return 0x66; };
    virtual UInt32 hardwareOutputQueueDepth(IO80211Interface*) { return 0; };
    virtual SInt32 performCountryCodeOperation(IO80211Interface*, IO80211CountryCodeOp) { return 0; };
    virtual bool useAppleRSNSupplicant(IO80211Interface *);
    virtual bool useAppleRSNSupplicant(IO80211VirtualInterface *);
    virtual void dataLinkLayerAttachComplete(IO80211Interface *);
	virtual SInt32 enableFeature(IO80211FeatureCode, void*) { return 0; };
	virtual SInt32 setVirtualHardwareAddress(IO80211VirtualInterface *,ether_addr *);
    virtual SInt32 enableVirtualInterface(IO80211VirtualInterface *);
    virtual SInt32 disableVirtualInterface(IO80211VirtualInterface *);

    virtual bool requiresExplicitMBufRelease() { return false; };
    virtual bool flowIdSupported() { return false; };

    virtual IO80211FlowQueueLegacy* requestFlowQueue(FlowIdMetadata const*);
    virtual void releaseFlowQueue(IO80211FlowQueue *);

    virtual void getLogPipes(CCPipe**, CCPipe**, CCPipe**) {};

    virtual IOReturn enablePacketTimestamping(void) {
        return kIOReturnUnsupported;
    }

    virtual IOReturn disablePacketTimestamping(void) {
        return kIOReturnUnsupported;
    }

    virtual UInt32 selfDiagnosticsReport(int,char const*,unsigned int);

    virtual UInt32 getDataQueueDepth(OSObject *);

#if __IO80211_TARGET >= __MAC_11_0
	virtual bool isAssociatedToMovingNetwork(void) { return false; }
#endif

    virtual mbuf_flags_t inputPacket(mbuf_t);

    virtual SInt32 apple80211_ioctl_get(IO80211Interface *,IO80211VirtualInterface *,ifnet_t,void *);

    virtual SInt32 apple80211_ioctl_get(IO80211SkywalkInterface *,void *);

    virtual SInt32 apple80211_ioctl_set(IO80211Interface *,IO80211VirtualInterface *,IO80211SkywalkInterface *,void *);

    virtual SInt32 apple80211_ioctl_set(IO80211SkywalkInterface *,void*);

    virtual bool attachInterface(IOSkywalkInterface *,IOService *);

#if __IO80211_TARGET >= __MAC_11_0
	virtual bool detachInterface(IOSkywalkInterface *, bool);
#endif

	virtual IO80211VirtualInterface* createVirtualInterface(ether_addr *,unsigned int);
    virtual bool attachVirtualInterface(IO80211VirtualInterface **,ether_addr *,unsigned int,bool);
    virtual bool detachVirtualInterface(IO80211VirtualInterface *,bool);

    virtual IOReturn enable(IO80211SkywalkInterface *);

    virtual IOReturn disable(IO80211SkywalkInterface *);
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *) APPLE_KEXT_OVERRIDE;
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *) APPLE_KEXT_OVERRIDE;

    virtual void detachInterface(IONetworkInterface *, bool sync = false) APPLE_KEXT_OVERRIDE;
    virtual bool attachInterface(IONetworkInterface **, bool attach = true) APPLE_KEXT_OVERRIDE;
    virtual void stop(IOService *) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;

#if __IO80211_TARGET < __MAC_11_0
	virtual bool terminate(unsigned int) APPLE_KEXT_OVERRIDE;
#endif

    virtual bool init(OSDictionary *) APPLE_KEXT_OVERRIDE;
    virtual IOService* getProvider(void) const APPLE_KEXT_OVERRIDE;
/*
    virtual SInt32 apple80211_ioctl_get(IO80211Interface *,IO80211VirtualInterface *,IO80211SkywalkInterface *,void *) {
        return kIOReturnUnsupported;
    }
 */

	IO80211SkywalkInterface* getInfraInterface(void);
	IO80211ScanManager* getPrimaryInterfaceScanManager(void);

	IO80211ControllerMonitor* getInterfaceMonitor(void);


    IOReturn addReporterLegend(IOService *,IOReporter *,char const*,char const*);
    IOReturn removeReporterFromLegend(IOService *,IOReporter *,char const*,char const*);
    IOReturn unlockIOReporterLegend(void);
    void lockIOReporterLegend(void); // Suspected return type - int
    IOReturn logIOReportLogStreamSubscription(unsigned long long);
    IOReturn addIOReportLogStreamForProvider(IOService *,unsigned long long *);
    IOReturn addSubscriptionForThisReporterFetchedOnTimer(IOReporter *,char const*,char const*,IOService *) ;
    IOReturn addSubscriptionForProviderFetchedOnTimer(IOService *);
    void handleIOReporterTimer(IOTimerEventSource *);
    void setIOReportersStreamFlags(unsigned long long);
    void updateIOReportersStreamFrequency(void); // Suspected return type - int
    void setIOReportersStreamLevel(CCStreamLogLevel);


    void powerChangeGated(OSObject *,void *,void *,void *,void *);
	int copyOut(void const*,unsigned long long,unsigned long);

    //modified


	SInt32 getASSOCIATE_RESULT(IO80211Interface *,IO80211VirtualInterface *,IO80211SkywalkInterface *,apple80211_assoc_result_data * assoc_result);
    virtual int errnoFromReturn(int) APPLE_KEXT_OVERRIDE;
    virtual const char* stringFromReturn(int) APPLE_KEXT_OVERRIDE;

    virtual IONetworkInterface* createInterface(void) APPLE_KEXT_OVERRIDE;
    virtual IOReturn getHardwareAddress(IOEthernetAddress *) APPLE_KEXT_OVERRIDE;
    virtual IOWorkLoop* getWorkLoop(void) const APPLE_KEXT_OVERRIDE;
    virtual bool createWorkLoop(void) APPLE_KEXT_OVERRIDE;
    virtual IOOutputQueue* getOutputQueue(void) const APPLE_KEXT_OVERRIDE;

    virtual bool configureInterface(IONetworkInterface *) APPLE_KEXT_OVERRIDE;
    virtual IOReturn enable(IONetworkInterface *) APPLE_KEXT_OVERRIDE;
    virtual IOReturn disable(IONetworkInterface *) APPLE_KEXT_OVERRIDE;
    virtual bool start(IOService *) APPLE_KEXT_OVERRIDE;

	virtual IOReturn outputStart(IONetworkInterface *,unsigned int);

    /*
    virtual IOReturn setChanNoiseFloorLTE(apple80211_stat_report *,int) {
        return kIOReturnSuccess;
    }
    virtual IOReturn setChanNoiseFloor(apple80211_stat_report *,int) {
        return kIOReturnSuccess;
    }
    virtual IOReturn setChanCCA(apple80211_stat_report *,int) {
        return kIOReturnSuccess;
    }
    virtual IOReturn setChanExtendedCCA(apple80211_stat_report *,apple80211_cca_report *) {
        return kIOReturnSuccess;
    }
    virtual bool setLTECoexstat(apple80211_stat_report *,apple80211_lteCoex_report *) {
        return false;
    }
    virtual bool setBTCoexstat(apple80211_stat_report *,apple80211_btCoex_report *) {
        return false;
    }
    virtual bool setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *,apple80211_channel *) {
        return false;
    }
    virtual UInt32 getCountryCode(apple80211_country_code_data *) {
        return 0;
    }
    virtual IOReturn setCountryCode(apple80211_country_code_data *) {
        return kIOReturnUnsupported;
    }
    virtual bool getInfraExtendedStats(apple80211_extended_stats *) {
        return false;
    }
    virtual bool getChipCounterStats(apple80211_chip_stats *) {
        return false;
    }
    virtual bool setExtendedChipCounterStats(apple80211_stat_report *,void *) {
        return false;
    }
    bool setChipCounterStats(apple80211_stat_report *,apple80211_chip_stats *,apple80211_channel *) {
        return false;
    }
    virtual bool setLeakyAPStats(apple80211_leaky_ap_event *) {
        return false;
    }
    bool setFrameStats(apple80211_stat_report *,apple80211_frame_counters *,apple80211_channel *) {
        return false;
    }
    bool setPowerStats(apple80211_stat_report *,apple80211_power_debug_sub_info *) {
        return false;
    }
     */

	IOReturn copyIn(unsigned long long,void *,unsigned long);
    void logIOCTL(apple80211req *);
	bool isIOCTLLoggingRestricted(apple80211req *);

	bool getBeaconPeriod(apple80211_beacon_period_data *);
	SInt32 apple80211VirtualRequestIoctl(unsigned int,int,IO80211VirtualInterface *,void *);
	bool getBSSIDData(OSObject *,apple80211_bssid_data *);
	bool getSSIDData(apple80211_ssid_data *);
	bool inputInfraPacket(mbuf_t);
    void notifyHostapState(apple80211_hostap_state *);
	bool isAwdlAssistedDiscoveryEnabled(void);
    void joinDone(scanSource,joinStatus);
    void joinStarted(scanSource,joinStatus);
    void handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *);
    void scanDone(scanSource,int);
    void scanStarted(scanSource,apple80211_scan_data *);
    void printChannels(void);
    void updateInterfaceCoexRiskPct(unsigned long long);
	SInt32 getInfraChannel(apple80211_channel_data *);
    void calculateInterfacesAvaiability(void); // Suspected return type - int
    void setChannelSequenceList(apple80211_awdl_sync_channel_sequence *); // Suspected return type - int
    void setPrimaryInterfaceDatapathState(bool);
	UInt32 getPrimaryInterfaceLinkState(void);
    void setCurrentChannel(apple80211_channel *); // Suspected return type - int
    void setHtCapability(ieee80211_ht_capability_ie *);
    UInt32 getHtCapability(void);
    UInt32 getHtCapabilityLength(void);
	bool io80211isDebuggable(bool* enable);
    void logDebug(unsigned long long,char const*,...); // Suspected return type - int
    void vlogDebug(unsigned long long,char const*,va_list); // Suspected return type - char
    void logDebug(char const*,...); // Suspected return type - int

	bool calculateInterfacesCoex(void);
	void setInfraChannel(apple80211_channel *);

    void configureAntennae(void);
    SInt32 apple80211RequestIoctl(unsigned int,int,IO80211Interface *,void *);
    UInt32 radioCountForInterface(IO80211Interface *);
    void releaseIOReporters(void);
	bool findAndAttachToFaultReporter(void);
    UInt32 setupControlPathLogging(void);
    IOReturn createIOReporters(IOService *);
    IOReturn powerChangeHandler(void *,void *,unsigned int,IOService *,void *,unsigned long);


    OSMetaClassDeclareReservedUnused( IO80211Controller,  0);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  1);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  2);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  3);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  4);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  5);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  6);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  7);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  8);
    OSMetaClassDeclareReservedUnused( IO80211Controller,  9);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 10);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 11);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 12);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 13);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 14);
    OSMetaClassDeclareReservedUnused( IO80211Controller, 15);

protected:
    static IORegistryPlane gIO80211Plane;
    static IORegistryEntry* kIO80211PlaneName;
    // 0x118
    IOTimerEventSource *        _report_gathering_timer;       // 0x118
    OSArray *                   _reporter_num;                 // 0x120 OSArray of OSNumber
    UInt32                      _var_128;                      // timeout ticks
    bool                        _wan_debug_enable;             // 0x12c
    // 3 bytes padding
    UInt32                      _debug_value;                  // 0x130
    IORecursiveLock *           _recursive_lock;               // 0x138
    UInt64                      _ht_cap_0x0;                   // 0x140
    UInt64                      _ht_cap_0x8;                   // 0x148
    UInt64                      _ht_cap_0x10;                  // 0x150
    UInt32                      _ht_cap_0x18;                  // 0x158
    UInt32                      _ht_cap_len;                   // 0x15c
    IO80211ControllerMonitor *  _fControllerMonitor;           // 0x160
    CCLogPipe *                 _fControllerIOReporterPipe;    // 0x168
    CCIOReporterLogStream *     _fControllerIOReporterStream;  // 0x170
    CCLogPipe *                 _controlPathLogPipe;           // 0x180
    CCLogStream *               _ioctlLogStream;               // 0x188
    CCLogStream *               _eventLogStream;               // 0x190
    IO82110WorkLoop *           _workLoop;                     // 0x198
    IO80211Interface *          _interface;                    // 0x1a0
    IO80211VirtualInterface *   _v_interface;                  // 0x1a8
    IO80211VirtualInterface     (* _vir_interface)[4];         // 0x1b0

    UInt64                      _vlog_debug;                   // 0x1d0 vlogDebug ?
    UInt32                      _unknown;                      // 0x1d8
    UInt32                      _infra_channel;                // 0x1dc compared with offset 8 of apple80211_stat_report  IO80211Controller::setChanCCA(apple80211_stat_report*, int)
    UInt32                      _infra_channel_flags;          // 0x1e0 compared with offset 8 of apple80211_channel
    UInt32                      _current_channel;              // 0x1e8 loaded with offset 04 of apple80211_channel
    UInt32                      _current_channel_flags;        // 0x1ec loaded with offset 08 of apple80211_channel
    UInt8                       _awdl_sync[0x190];             // 0x1f0, 0x190 bytes apple80211_awdl_sync_channel_sequence
    IONotifier *                _powerDownNotifier;            // 0x380
    IOService  *                _provider;                     // 0x388
    IO80211RangingManager *     _ranger_manager;               // 0x390
    bool                        _var_398;                      // 0x398 checked in IO80211Controller::disable(IONetworkInterface*)
    // 7 byte padding
    IONotifier *                _notifier1;                    // 0x3a0
    bool                        _var_3a8;                      // 0x3a8
    // 7 byte padding
    UInt64                      _last_pointer;                 // 0x3b0  unused
    uint8_t                     filler[0x2B2];
    //0x3CA
};

// 0x215: 1 byte, length of channel sequence, should be 16
// 0x21c: channel sequence, should contain 16 elements of length 12, possibly apple80211_channel (but why 16?)
// struct of three ints, last looks like flags, first unused

/*
 void __thiscall
setChannelSequenceList(IO80211Controller *this,apple80211_awdl_sync_channel_sequence *param_1)

{
  _memcpy(this + 0x210,param_1,400);
  calculateInterfacesAvaiability(this);
  return;
}
*/


#endif /* defined(KERNEL) && defined(__cplusplus) */

#endif /* !_IO80211CONTROLLER_H */
