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
    UInt8 * Start;
    UInt8 * End;
    UInt8 * NextChar;
    UInt8 * LastChar;
    IOByteCount Size;
    IOByteCount InQueue;
    IOLock * InUse;
} CirQueue;

typedef enum QueueStatus
{
    queueNoError = 0,
    queueFull,
    queueEmpty,
    queueMaxStatus
} QueueStatus;

QueueStatus             InitQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
QueueStatus             CloseQueue(CirQueue * Queue);
size_t                  AddtoQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
size_t                  FreeSpaceinQueue(CirQueue * Queue);
QueueStatus             PrivateAddBytetoQueue(CirQueue * Queue, char Value);
size_t                  GetSingleBlockPointer(CirQueue * Queue, size_t MaxSize, UInt8 ** Pointer);
size_t                  ConsiderThisBlockRead(CirQueue * Queue, UInt8 * Buffer, size_t Size);

size_t                  FillBlockWithQueue(CirQueue * Queue, IOBluetoothMemoryBlock * Block);
size_t                  RemovefromQueue(CirQueue * Queue, UInt8 * Buffer, size_t Size);
QueueStatus             PrivateGetBytetoQueue(CirQueue * Queue, UInt8 * Value);
size_t                  UsedSpaceinQueue(CirQueue * Queue);
size_t                  GetQueueSize(CirQueue * Queue);
QueueStatus             AddBytetoQueue(CirQueue * Queue, char Value);
QueueStatus             GetBytetoQueue(CirQueue * Queue, UInt8 * Value);
QueueStatus             GetQueueStatus(CirQueue * Queue);

typedef struct Stats_t
{
    UInt32 ints;
    UInt32 txInts;
    UInt32 rxInts;
    UInt32 mdmInts;
    UInt32 txChars;
    UInt32 rxChars;
} Stats_t;

typedef struct BufferMarks
{
    UInt32    BufferSize;
    UInt32    HighWater;
    UInt32    LowWater;
    bool    OverRun;
} BufferMarks;

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

    // dbdma memory control
    IOLock            *IODBDMARxLock;
    IOLock            *IODBDMATrLock;

    IOLock            *SCCAccessLock;

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

protected:
    //136
    IOService * mProvider; //536
};

/*
IOBluetoothSerialClient::changeState(PortInfo_t *,uint,uint)
IOBluetoothSerialClient::dataInFunction(IOService *,ushort,void *)
IOBluetoothSerialClient::readPortState(PortInfo_t *)
IOBluetoothSerialClient::CheckQueues(PortInfo_t *)
IOBluetoothSerialClient::BlockIsGone(IOBluetoothMemoryBlock *,int,ulong long,ulong long,ulong long,ulong long,ulong long)
IOBluetoothSerialClient::SendNextBlock(PortInfo_t *)
IOBluetoothSerialClient::SetUpTransmit(void *,void *)
IOBluetoothSerialClient::detachRFCOMMLink(PortInfo_t *)

IOBluetoothSerialClient::SetStructureDefaults(PortInfo_t *,bool)
IOBluetoothSerialClient::allocateRingBuffer(CirQueue *,ulong)
IOBluetoothSerialClient::createSerialStream(void)

IOBluetoothSerialClient::deactivatePort(PortInfo_t *)

IOBluetoothSerialClient::freeRingBuffer(CirQueue *)

IOBluetoothSerialClient::acquirePort(bool,void *)
IOBluetoothSerialClient::releasePort(void *)
IOBluetoothSerialClient::setState(uint,uint,void *)
IOBluetoothSerialClient::getState(void *)
IOBluetoothSerialClient::watchState(uint *,uint,void *)
IOBluetoothSerialClient::watchState(PortInfo_t *,uint *,uint)
IOBluetoothSerialClient::nextEvent(void *)
IOBluetoothSerialClient::executeEvent(uint,uint,void *)
IOBluetoothSerialClient::executeEvent(PortInfo_t *,uint,uint,uint *,uint *)
IOBluetoothSerialClient::requestEvent(uint,uint *,void *)
IOBluetoothSerialClient::enqueueEvent(uint,uint,bool,void *)
IOBluetoothSerialClient::dequeueEvent(uint *,uint *,bool,void *)
IOBluetoothSerialClient::enqueueData(uchar *,uint,uint *,bool,void *)
IOBluetoothSerialClient::dequeueData(uchar *,uint,uint *,uint,void *)
IOBluetoothSerialClient::activatePort(PortInfo_t *)
IOBluetoothSerialClient::dataLatTOHandler(PortInfo_t *)
IOBluetoothSerialClient::frameTOHandler(PortInfo_t *)
IOBluetoothSerialClient::delayTOHandler(PortInfo_t *)
IOBluetoothSerialClient::heartBeatTOHandler(PortInfo_t *)
*/

#endif
