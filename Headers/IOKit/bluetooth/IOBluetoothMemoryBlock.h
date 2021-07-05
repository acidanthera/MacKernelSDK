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

#ifndef _IOBLUETOOTH_MEMORYBLOCK_H
#define _IOBLUETOOTH_MEMORYBLOCK_H

#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOTimerEventSource.h>

class IOBluetoothMemoryBlock : public OSObject
{
    OSDeclareAbstractStructors(IOBluetoothMemoryBlock)
    
    typedef void (* IOBluetoothMemoryBlockCallBack)(IOBluetoothMemoryBlock *, int, UInt64, UInt64, UInt64, UInt64, UInt64);
    
public:
    static IOBluetoothMemoryBlock * withAddress( void * address, IOByteCount withLength, IODirection withDirection );
    static IOBluetoothMemoryBlock * withCapacity( IOByteCount capacity, IODirection direction );
    static IOBluetoothMemoryBlock * withSubBlock( IOBluetoothMemoryBlock * subBlock, IODirection direction );
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual bool initWithAddress( void * address, IOByteCount withLength, IODirection withDirection );
    virtual bool initWithCapacity( IOByteCount capacity, IODirection direction );
    virtual bool initWithSubBlock( IOBluetoothMemoryBlock * subBlock, IODirection direction );
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual UInt64 readBytes( UInt64 offset, void * bytes, UInt64 length );
    virtual UInt64 writeBytes( UInt64 offset, const void * bytes, UInt64 length );
    virtual IODirection getDirection();
    virtual UInt64 getLength();
    virtual IOBluetoothMemoryBlock * getBlock();
    virtual IOReturn appendBlock( IOBluetoothMemoryBlock * block );
    virtual IOReturn setCallBack( IOBluetoothMemoryBlockCallBack callBack, UInt64 data1, UInt64 data2, UInt64 data3, UInt64 data4, UInt64 data5 );
    virtual bool hasCallBack();
    virtual bool sent( int );
    virtual int packetSentStatus();
    
protected:
    //8 bytes for vtable pointer
    //8 bytes inherited from OSObject
    void * mBytes; //this + 16, address in initWithAddress
    UInt64 mBytesLength; //this + 24, length
    bool mIsWithCapacity; //this + 32
    uint8_t reserved1[7]; //7 bytes padding
    IOBluetoothMemoryBlock * mSubBlock; //this + 40
    IODirection mDirection; //this + 48
    uint32_t reserved2; //4 byte padding
    IOBluetoothMemoryBlockCallBack mCallBack; //this + 56
    UInt64 mCallBackData1; //this + 64
    UInt64 mCallBackData2; //this + 72
    UInt64 mCallBackData3; //this + 80
    UInt64 mCallBackData4; //this + 88
    UInt64 mCallBackData5; //this + 96
    int mPacketSentStatus; //this + 104
} __attribute__((packed));

typedef UInt16 IOBluetoothL2CAPChannelID;

class IOBluetoothL2CAPMemoryBlock : public IOBluetoothMemoryBlock
{
    OSDeclareAbstractStructors(IOBluetoothL2CAPMemoryBlock)
    
public:
    static IOBluetoothL2CAPMemoryBlock * withAddress( void * address, IOByteCount withLength, IODirection withDirection );
    static IOBluetoothL2CAPMemoryBlock * withCapacity( IOByteCount capacity, IODirection direction );
    static IOBluetoothL2CAPMemoryBlock * withSubBlock( IOBluetoothMemoryBlock * subBlock, IODirection direction );
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;

    virtual UInt16 readChannelID();
    virtual void writeChannelID(IOBluetoothL2CAPChannelID channelID);
    virtual UInt64 readBytes( UInt64 offset, void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 writeBytes( UInt64 offset, const void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 getLength() APPLE_KEXT_OVERRIDE;
    
protected:
    //108 bytes inherited from IOBluetoothMemoryBlock
    IOBluetoothL2CAPChannelID mChannelID; //this + 108
} __attribute__((packed));

UInt16 IOBluetoothL2CAPFCS(UInt16, UInt8);

class IOBluetoothL2CAPSupervisoryFrameMemoryBlock : public IOBluetoothL2CAPMemoryBlock
{
    OSDeclareDefaultStructors(IOBluetoothL2CAPSupervisoryFrameMemoryBlock)
    
public:
    static IOBluetoothL2CAPSupervisoryFrameMemoryBlock * withDirection( IODirection direction );
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool sent( int ) APPLE_KEXT_OVERRIDE;
    virtual bool hasCallBack() APPLE_KEXT_OVERRIDE;
    virtual UInt64 readBytes( UInt64 offset, void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 writeBytes( UInt64 offset, const void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn readFCS( UInt16 * );
    virtual IOReturn writeFCS( UInt16 );
    virtual UInt64 getLength() APPLE_KEXT_OVERRIDE;
    virtual UInt8 getSupervisoryFunctionMask( UInt16 );
    virtual UInt8 getSupervisoryFunctionMask();
    virtual UInt8 setSupervisoryFunctionMask( UInt8 );
    virtual UInt8 getRetransmissionDisableBit( UInt16 );
    virtual UInt8 getRetransmissionDisableBit();
    virtual UInt8 setRetransmissionDisableBit( bool );
    virtual UInt8 getReceiveSequenceNumber( UInt16 );
    virtual UInt8 getReceiveSequenceNumber();
    virtual UInt8 setReceiveSequenceNumber( UInt8 );
    
protected:
    //110 bytes inherited from IOBluetoothL2CAPMemoryBlock
    UInt8 mSupervisoryFunctionMask; //this + 110
    //The first bit of this UInt8 stores the RetransmissionDisableBit, and the 5 & 6 bits stores the SupervisoryFunctionMask. The other bits, as of right now, are reserved.
    UInt8 mReceiveSequenceNumber; //this + 111
    UInt16 mFCS; //this + 112, Frame Check Sequence
};

class IOBluetoothL2CAPInformationFrameMemoryBlock : public IOBluetoothL2CAPMemoryBlock
{
    OSDeclareDefaultStructors(IOBluetoothL2CAPInformationFrameMemoryBlock)
    
public:
    static IOBluetoothL2CAPInformationFrameMemoryBlock * withDataFromBlock( IOBluetoothL2CAPInformationFrameMemoryBlock * block, UInt64, UInt64, UInt32 );
    virtual bool initWithDataFromBlock( IOBluetoothL2CAPInformationFrameMemoryBlock * block, UInt64, UInt64, UInt32 );
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual UInt16 getSegmentationMask( UInt16 );
    virtual UInt16 getSegmentationMask();
    virtual UInt16 setSegmentationMask( UInt16 );
    virtual UInt16 getTransmitSequenceNumber( UInt16 );
    virtual UInt16 getTransmitSequenceNumber();
    virtual UInt16 setTransmitSequenceNumber( UInt8 );
    virtual UInt16 getSDU();
    virtual int setSDU( UInt16 );
    virtual UInt64 readBytes( UInt64 offset, void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 writeBytes( UInt64 offset, const void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual IOBluetoothMemoryBlock * getBlock() APPLE_KEXT_OVERRIDE;
    virtual UInt64 getLength() APPLE_KEXT_OVERRIDE;
    virtual UInt16 getPayload();
    virtual bool sent( int ) APPLE_KEXT_OVERRIDE;
    virtual int acked( int );
    virtual bool hasCallBack() APPLE_KEXT_OVERRIDE;
    
protected:
    //110 bytes inherited from IOBluetoothL2CAPInformationFrameMemoryBlock
    UInt16 mSegmentationAndReassemblyMask;  //(unsigned __int16 *)(this + 110)
    UInt16 unknown1; //*(_WORD *)(this + 112)
    UInt16 unknown2;  //*(_WORD *)(this + 114)
    UInt16 mPayload; //*(_WORD *)(this + 116)
    UInt16 mSDU;  //*(_WORD *)(this + 118)
};

class IOBluetoothRFCOMMMemoryBlock : public IOBluetoothMemoryBlock
{
    OSDeclareDefaultStructors(IOBluetoothRFCOMMMemoryBlock)
    
public:
    static IOBluetoothRFCOMMMemoryBlock * withAddress( void * address, IOByteCount withLength, IODirection withDirection );
    static IOBluetoothRFCOMMMemoryBlock * withCapacity( IOByteCount capacity, IODirection direction );
    static IOBluetoothRFCOMMMemoryBlock * withSubBlock( IOBluetoothMemoryBlock * subBlock, IODirection direction );
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    virtual UInt8 readAddressByte();
    virtual void writeAddressByte( UInt8 whatByte );
    virtual UInt8 readControlByte();
    virtual void writeControlByte( UInt8 whatByte );
    virtual UInt8 readFCFByte();
    virtual void writeFCFByte( UInt8 whatByte );
    virtual UInt8 readHeaderBytes( void * bytes, UInt64 length );
    virtual UInt64 readBytes( UInt64 offset, void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 writeBytes( UInt64 offset, const void * bytes, UInt64 length ) APPLE_KEXT_OVERRIDE;
    virtual UInt64 getLength() APPLE_KEXT_OVERRIDE;
    virtual UInt64 getLogicalLength();
    
protected:
    //108 bytes inherited from IOBluetoothMemoryBlock
    UInt8 mAddressByte; //this + 108
    UInt8 mControlByte; //this + 109
    UInt16 unknown1; //this + 110
    UInt8 mHeaderBytes; //this + 112
    UInt8 mFCFByte; //this + 113
};

class IOBluetoothACLMemoryDescriptor : public IOGeneralMemoryDescriptor
{
    OSDeclareDefaultStructors(IOBluetoothACLMemoryDescriptor)
    
public:
    static IOBluetoothACLMemoryDescriptor * withL2CAPMemoryBlock( IOBluetoothL2CAPMemoryBlock * block, UInt64, UInt64 withOffset, UInt16, IOOptionBits options, bool asReference = false );
    virtual bool initWithAddress( void * address, IOByteCount withLength, IODirection withDirection );
    virtual bool initWithAddress( IOVirtualAddress address, IOByteCount withLength, IODirection withDirection, task_t withTask );
    virtual bool initWithPhysicalAddress( IOPhysicalAddress address, IOByteCount withLength, IODirection withDirection );
    virtual bool initWithPhysicalRanges( IOPhysicalRange * ranges, UInt32 withCount, IODirection withDirection, bool asReference = false );
    virtual bool initWithRanges( IOVirtualRange * ranges, UInt32 withCount, IODirection withDirection, task_t withTask, bool asReference = false );
    virtual bool initWithL2CAPMemoryBlock( IOBluetoothL2CAPMemoryBlock * block, UInt64, UInt64 withOffset, UInt16, IOOptionBits options, bool asReference = false );
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool prepareHCIBlock();
    
    virtual IOByteCount getLength();
    virtual IOByteCount getPayloadLength();
    virtual IOByteCount readBytes( IOByteCount offset, void * bytes, IOByteCount withLength ) APPLE_KEXT_OVERRIDE;
    virtual IOByteCount writeBytes( IOByteCount offset, const void * bytes, IOByteCount withLength ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn prepare(IODirection forDirection = kIODirectionNone) APPLE_KEXT_OVERRIDE;
    virtual IOReturn complete(IODirection forDirection = kIODirectionNone) APPLE_KEXT_OVERRIDE;
    virtual bool sent( int );
    virtual int packetSentStatus();
    virtual bool hasCallBack();
    virtual void setPacketSentStatusToInvalid();
    
protected:
    //8 bytes for virtual function pointer table
    //168 inherited
    IOBluetoothL2CAPMemoryBlock * mBlock; //*(_QWORD *)(this + 176)
    IOByteCount mBuffersOffset; //*(_QWORD *)(this + 184)
    IOByteCount mLength; //*(_QWORD *)(this + 192)
    UInt16 unknown1; //*(_WORD *)(this + 200)
    UInt16 unknown2; //*(_WORD *)(this + 202)
    UInt32 unknown3; //*(_DWORD *)(this + 204)
    bool mAsReference; //*(_BYTE *)(this + 208)
    uint8_t reserved1[3];
    int mPacketSentStatus; //*(_DWORD *)(this + 212)
    void * mBuffers; //*(_QWORD *)(this + 216)
    IOByteCount mBuffersSize; //*(_QWORD *)(this + 224)
    uint64_t reserved2[2]; //this + 232
    uint16_t reserved3; //*(_WORD *)(this + 248)
    uint8_t reserved4; //*(_BYTE *)(this + 250)
};

class IOBluetoothMemoryDescriptorRetainer : public OSObject
{
    OSDeclareDefaultStructors(IOBluetoothMemoryDescriptorRetainer)
    
public:
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;

protected:
    IOMemoryDescriptor * reserved1; //16
    IOMemoryDescriptor * reserved2; //24
    void * owner;                   //32
    IOTimerEventSource * timer; //40
};

class IOBluetoothSCOMemoryDescriptorRetainer : public OSObject
{
    OSDeclareDefaultStructors(IOBluetoothSCOMemoryDescriptorRetainer)
    
public:
    virtual bool init() APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    
protected:
    //8 bytes for virtual function pointer
    //8 bytes inherited from OSObject
    UInt64 reserved1; //*(_QWORD *)(this + 16)
    UInt64 reserved2; //*(_QWORD *)(this + 24)
    UInt64 reserved3; //*(_QWORD *)(this + 32)
    UInt64 reserved4; //*(_QWORD *)(this + 40)
    UInt64 reserved5; //*(_QWORD *)(this + 48)
    UInt64 reserved6; //*(_QWORD *)(this + 56)
    UInt64 reserved7; //*(_QWORD *)(this + 64)
    UInt64 reserved8; //*(_QWORD *)(this + 72)
};

#endif
