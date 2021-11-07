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

#ifndef _IOKIT_IOBLUETOOTHSERIALCLIENT_H
#define _IOKIT_IOBLUETOOTHSERIALCLIENT_H

#include <IOKit/serial/IOModemSerialStreamSync.h>
#include <IOKit/serial/IORS232SerialStreamSync.h>
#include <IOKit/serial/IOSerialDriverSync.h>
#include <IOKit/bluetooth/IOBluetoothMemoryBlock.h>

typedef struct CirQueue
{
    UInt8 *     Start;
    UInt8 *     End;
    UInt8 *     NextChar;
    UInt8 *     LastChar;
    IOByteCount Size;
    IOByteCount InQueue;
    IOLock *    InUse;
} CirQueue;

typedef enum QueueStatus
{
    queueNoError = 0,
    queueFull,
    queueEmpty,
    queueMaxStatus
} QueueStatus;

QueueStatus InitQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
QueueStatus CloseQueue(CirQueue * Queue);
size_t      AddtoQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
size_t      FreeSpaceinQueue(CirQueue * Queue);
QueueStatus PrivateAddBytetoQueue(CirQueue * Queue, char Value);
size_t      GetSingleBlockPointer(CirQueue * Queue, size_t MaxSize, UInt8 ** Pointer);
size_t      ConsiderThisBlockRead(CirQueue * Queue, UInt8 * Buffer, size_t Size);

size_t      FillBlockWithQueue(CirQueue * Queue, IOBluetoothMemoryBlock * Block);
size_t      RemovefromQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
QueueStatus PrivateGetBytetoQueue(CirQueue * Queue, UInt8 * Value);
size_t      UsedSpaceinQueue(CirQueue * Queue);
size_t      GetQueueSize(CirQueue * Queue);
QueueStatus AddBytetoQueue(CirQueue * Queue, char Value);
QueueStatus GetBytetoQueue(CirQueue * Queue, UInt8 * Value);
QueueStatus GetQueueStatus(CirQueue * Queue);

typedef struct BufferMarks
{
    UInt32  BufferSize;
    UInt32  HighWater;
    UInt32  LowWater;
    bool    OverRun;
} BufferMarks;

typedef struct Stats_t
{
    UInt32 ints;
    UInt32 txInts;
    UInt32 rxInts;
    UInt32 mdmInts;
    UInt32 txChars;
    UInt32 rxChars;
} Stats_t;

typedef struct PortInfo_t
{
    UInt32 Instance; //0
    unsigned const char * PortName; //8
    UInt32 State; //16
    UInt32 WatchStateMask; //20
    IOSimpleLock * WatchLock; //24
    IOSimpleLock * serialRequestLock; //32

    // queue control structures:
    CirQueue RX; //40
    CirQueue TX; //96

    BufferMarks RXStats; //152
    BufferMarks TXStats; //168

    //184
    //192
    //200

    // dbdma memory control
    IOLock            *IODBDMARxLock;
    IOLock            *IODBDMATrLock;

    // UART configuration info:
    UInt32            Base;
    //UInt32            IRQ;
    /* No Longer used ejk     */
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
    /* Indicates our receive state.
    */
    int                        RXOstate;
    /* Indicates our transmit state, if we have received any Flow Control.
    */
    int                        TXOstate;
    UInt8                    xOffSent;        /* ejk     */

    /* Globals in the Copeland Version.
    */
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

    /*
     *gDBDMAChannelAddress, gChannelCommandArea reference the DBDMA channel
     *command area. This is a kernel memory area.
     */
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
} PortInfo_t;

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
    OSDeclareDefaultStructors(IOBluetoothSerialClient)

public:
    static void serialSignalsCallBack(IOService * provider, int);

    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool requestTerminate(IOService * provider, IOOptionBits options) APPLE_KEXT_OVERRIDE;

    virtual IOService * probe(IOService * provider, SInt32 * score) APPLE_KEXT_OVERRIDE;
    virtual bool start(IOService * provider) APPLE_KEXT_OVERRIDE;
    virtual void stop(IOService * provider) APPLE_KEXT_OVERRIDE;
    virtual bool attach(IOService * provider) APPLE_KEXT_OVERRIDE;
    virtual void detach(IOService * provider) APPLE_KEXT_OVERRIDE;

    static void changeState(PortInfo_t * port, UInt32 state, UInt32 mask);
    static void dataInFunction(IOService * target, UInt16, void *);
    static UInt32 readPortState(PortInfo_t * port);
    static void CheckQueues(PortInfo_t * port);
    static bool BlockIsGone(IOBluetoothMemoryBlock * block, int, UInt64, UInt64, UInt64, UInt64, UInt64);
    bool SendNextBlock(PortInfo_t * port);
    bool SetUpTransmit(void * port, void * refCon);
    static void detachRFCOMMLink(PortInfo_t * port);

    static void SetStructureDefaults(PortInfo_t * port, bool init);
    bool createSerialStream();

    static bool allocateRingBuffer(CirQueue * Queue, size_t BufferSize);
    static void freeRingBuffer(CirQueue * Queue);

    virtual IOReturn acquirePort(bool sleep, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn releasePort(void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setState(UInt32 state, UInt32 mask, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual UInt32 getState(void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn watchState(UInt32 * state, UInt32 mask, void * refCon) APPLE_KEXT_OVERRIDE;
    static IOReturn watchState(PortInfo_t * port, UInt32 * state, UInt32 mask);
    virtual UInt32 nextEvent(void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn executeEvent(UInt32 event, UInt32 data, void * refCon) APPLE_KEXT_OVERRIDE;
    static IOReturn executeEvent(PortInfo_t *, UInt32 event, UInt32 data, UInt32 * oldState, UInt32 * newState);
    virtual IOReturn requestEvent(UInt32 event, UInt32 * data, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn enqueueEvent(UInt32 event, UInt32 data, bool sleep, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn dequeueEvent(UInt32 * event, UInt32 * data, bool sleep, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn enqueueData(UInt8 * buffer, UInt32 size, UInt32 * count, bool sleep, void * refCon) APPLE_KEXT_OVERRIDE;
    virtual IOReturn dequeueData(UInt8 * buffer, UInt32 size, UInt32 * count, UInt32 min, void * refCon) APPLE_KEXT_OVERRIDE;

    static IOReturn activatePort(PortInfo_t * port);
    static void deactivatePort(PortInfo_t * port);

    static void dataLatTOHandler(PortInfo_t * port);
    static void frameTOHandler(PortInfo_t * port);
    static void delayTOHandler(PortInfo_t * port);
    static void heartBeatTOHandler(PortInfo_t * port);

protected:
    PortInfo_t mSerialPort; //136
    IOService * mProvider; //536
};

#endif
