//
//  IOBluetoothSerialClient.hpp
//  IOBluetoothFamily
//
//  Created by mac on 2021/7/2.
//

#ifndef IOBluetoothSerialClient_h
#define IOBluetoothSerialClient_h

#include <IOKit/serial/IOSerialDriverSync.h>
#include <IOKit/serial/IOModemSerialStreamSync.h>
#include <IOKit/serial/IORS232SerialStreamSync.h>
//#include "CirQueue.h"

/*
typedef struct
{
    UInt32            Instance; //0
    void *            fAppleSCCSerialInstance; //8
    //unsigned const char        *PortName; //16
    UInt32              State; //16
    UInt32              WatchStateMask; //20
    IOSimpleLock *        WatchLock; //24
    IOSimpleLock *        serialRequestLock; //32

    //IOLock *        WatchLock;
    //IOLock             *serialRequestLock;
    //IORecursiveLock         *serialRequestLock;
    
    // queue control structures:
    CirQueue            RX;
    CirQueue            TX;
    BufferMarks            RXStats;
    BufferMarks            TXStats;

    // dbdma memory control
    IOLock            *IODBDMARxLock;
    IOLock            *IODBDMATrLock;

    IOLock                    *SCCAccessLock;
    
    // UART configuration info:
    UInt32            Base;
    //UInt32            IRQ;
    // No Longer used ejk
    //UInt32            Type;
    UInt32            CharLength;
    UInt32            StopBits;
    UInt32            TX_Parity;
    UInt32            RX_Parity;
    UInt32            BreakLength;
    UInt32            BaudRate;
    unsigned short        DLRimage;
    UInt8            LCRimage;
    UInt8            FCRimage;
    UInt8            IERmask;
    UInt8            RBRmask;
    UInt32            MasterClock;
    bool                    MinLatency;
    bool            WaitingForTXIdle;
    //bool                  JustDoneInterrupt;
    //bool            PCMCIA;
    //bool            PCMCIA_yanked;

    // flow control state & configuration:
    UInt8                    XONchar;
    UInt8                    XOFFchar;
    UInt32                    SWspecial[(0x100)>>SPECIAL_SHIFT];
    UInt32                    FlowControl;    // notify-on-delta & auto_control
    UInt32                    FlowControlState;
    bool                    DCDState;
    bool                    CTSState;
    // Indicates our receive state.
    int                        RXOstate;
    // Indicates our transmit state, if we have received any Flow Control.
    int                        TXOstate;
    UInt8                    xOffSent;        // ejk

    // Globals in the Copeland Version.
    UInt32                     GlobalRecvChars;
    UInt32                  OverRunChars;
    //UInt32 TrChars;

    // callout entries:
    IOThread                FrameTOEntry;
    IOThread                DataLatTOEntry;
    IOThread                DelayTOEntry;
    IOThread                HeartBeatTOEntry;
    mach_timespec            FrameInterval;
    mach_timespec            DataLatInterval;
    mach_timespec            CharLatInterval;
    mach_timespec            HeartBeatInterval;
    // Statistics
    Stats_t                    Stats;
    bool                    AreTransmitting;
    bool                    GotTXInterrupt;

    // chip dependent
    bool                    baudRateGeneratorEnable;
    UInt8                    baudRateGeneratorLo;
    UInt8                    baudRateGeneratorHi;
    UInt32                    rtxcFrequency;
    UInt8                    lastWR[ kNumSCCWR ];

    UInt8                    *ControlRegister;
    UInt8                    *DataRegister;
    IOPhysicalAddress        ChipBaseAddress;
    UInt32                    ConfigWriteRegister;
    SerialPortSelector        whichPort;
    IOPhysicalAddress        TxDMABase;
    UInt32                    TxDMALength;
    IOPhysicalAddress        RxDMABase;
    UInt32                    RxDMALength;

    UInt8                    InterruptNumbers[MaxInterrupts];

    // enable bits
    UInt8                    ourReceiveBits;
    UInt8                    ourTransmitBits;
    UInt8                    ourClockSource;
    bool                    haveQueuedRxSIH;
    bool                    haveQueuedTxSIH;

    // gDBDMAChannelAddress, gChannelCommandArea reference the DBDMA channel command area. This is a kernel memory area.
    SerialDBDMAStatusInfo    TxDBDMAChannel;
    SerialDBDMAStatusInfo    RxDBDMAChannel;
    
    bool                    DTRAsserted;
    bool                    aboveRxHighWater;
    
    bool                    lastCTSState;
    UInt32                    lastCTSTime;
    UInt32                    ctsTransitionCount;
    
    
    IOTimerEventSource*        rxTimer;
    // for DCP -- begin
    
    bool                    gDCPUserClientSet;
    bool                    gDCPModemFound;
    DCPModemSupport            *DCPModemSupportPtr;
    
    // for DCP -- end

} PortInfo_t; */

struct PortInfo_t;
struct CirQueue;

class IOBluetoothSerialClientModemStreamSync : public IOModemSerialStreamSync
{
    OSDeclareDefaultStructors(IOBluetoothSerialClientModemStreamSync)
    
public:
    virtual bool compareName( OSString * name, OSString ** matched = NULL ) const APPLE_KEXT_OVERRIDE;
};

class IOBluetoothSerialClientSerialStreamSync : public IORS232SerialStreamSync
{
    OSDeclareDefaultStructors(IOBluetoothSerialClientSerialStreamSync)
    
public:
    virtual bool compareName( OSString * name, OSString ** matched = NULL ) const APPLE_KEXT_OVERRIDE;
};

class IOBluetoothSerialClient : public IOSerialDriverSync
{
    OSDeclareAbstractStructors(IOBluetoothSerialClient)
    
public:
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual IOService * probe( IOService * provider, SInt32 * score ) APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool attach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void detach( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool requestTerminate( IOService * provider, IOOptionBits options ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn acquirePort( bool sleep, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn releasePort( void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setState( UInt32 state, UInt32 mask, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual UInt32 getState( void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn watchState( UInt32 * state, UInt32 mask, void * refCon ) APPLE_KEXT_OVERRIDE;
    IOReturn watchState( PortInfo_t * port, UInt32 * state, UInt32 mask );
    virtual UInt32 nextEvent( void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn executeEvent( UInt32 event, UInt32 data, void * refCon ) APPLE_KEXT_OVERRIDE;
    IOReturn executeEvent( PortInfo_t * port, UInt32 event, UInt32 data, UInt32 * PortState, UInt32 * );
    virtual IOReturn requestEvent( UInt32 event, UInt32 * data, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn enqueueEvent( UInt32 event, UInt32 data, bool sleep, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn dequeueEvent( UInt32 * event, UInt32 * data, bool sleep, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn enqueueData( UInt8 * buffer, UInt32 size, UInt32 * count, bool sleep, void * refCon ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn dequeueData( UInt8 * buffer, UInt32 size, UInt32 * count, UInt32 min, void * refCon ) APPLE_KEXT_OVERRIDE;
    
    bool activatePort( PortInfo_t * port );
    bool deactivatePort( PortInfo_t * port );
    bool changeState( PortInfo_t * port, UInt32 state, UInt32 mask );
    UInt32 readPortState( PortInfo_t * port );
    
    static void serialSignalsCallBack(IOService *, int);
    bool createSerialStream();
    static void dataInFunction(IOService * owner, UInt16, void *);
    
    //CheckQueues(PortInfo_t *);
    //BlockIsGone(IOBluetoothMemoryBlock *,int,ulong long,ulong long,ulong long,ulong long,ulong long);
    //SendNextBlock(PortInfo_t *);
    //SetUpTransmit(void *,void *);
    //detachRFCOMMLink(PortInfo_t *);
    
    void SetStructureDefaults(PortInfo_t *, bool);
    
    bool allocateRingBuffer(CirQueue *, size_t);
    void freeRingBuffer(CirQueue *);

    //dataLatTOHandler(PortInfo_t *)
    //frameTOHandler(PortInfo_t *)
    //delayTOHandler(PortInfo_t *)
    //heartBeatTOHandler(PortInfo_t *)
};

#endif
