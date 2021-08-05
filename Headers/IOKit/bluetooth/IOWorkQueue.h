/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
 * Copyright (c) 2021 cjiang.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IOWORKQUEUE_H
#define _IOKIT_IOWORKQUEUE_H

#include <IOKit/IOCommandGate.h>
#include <IOKit/IOLib.h>

/*! @class IOWorkQueue : public IOEventSource
 *   @abstract A circular queue that contains IOWorkQueueCall instances as its entries.
 *   @discussion ???
 */

class IOWorkQueue : public IOEventSource
{
    OSDeclareAbstractStructors(IOWorkQueue)
    
    typedef IOReturn (*Action)(OSObject * owner, void * arg0, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5);
    
    //TO-DO: Make this clear
    typedef int IOWorkQueueOperationMode;
    enum IOWorkQueueOperationModes
    {
        kIOWorkQueueOperationModeWorkAvailable = 0,
        kIOWorkQueueOperationModeWorkNotAvailable = 1,
        kIOWorkQueueOperationModeCount
    };
    
    struct IOWorkQueueCall
    {
        Action              action; //0
        void *              arg0; //8
        void *              arg1; //16
        void *              arg2; //24
        void *              arg3; //32
        void *              arg4; //40
        void *              arg5; //48
        
        IOWorkQueueCall *   nextCall; //56
        IOWorkQueueCall *   previousCall; //64
    };
    
public:
    virtual void unusedCall002(IOWorkQueueCall **, IOWorkQueueCall **, IOWorkQueueCall *);
    virtual void unusedCall003(IOWorkQueueCall **, IOWorkQueueCall **);
    
    virtual void enqueueWorkCall(IOWorkQueueCall *);
    virtual IOWorkQueueCall * dequeueWorkCall();
    
    virtual void unusedCall000(IOWorkQueueCall *);
    virtual void unusedCall001();
    
    virtual bool checkForWork() APPLE_KEXT_OVERRIDE;
    virtual IOReturn executeWorkCall(IOWorkQueueCall *);
    
    virtual void processWorkCallFromSeparateThread(IOWorkQueueCall *);
    virtual void processWorkCallFromSeparateThreadWL();
    static void * ThreadCallMain(void *, wait_result_t);
    
    virtual void unusedCall004(IOWorkQueueCall *);
    
    static IOWorkQueue * withCapacity(OSObject * owner, UInt32 size, IOWorkQueueOperationMode mode, bool isDebug, char * workQueueName);
    virtual bool initWithCapacity(OSObject * owner, UInt32 size, IOWorkQueueOperationMode mode, bool isDebug, char * workQueueName);
    
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn enqueueAction(Action action, void * arg0, void * arg1, void * arg2, void * arg3, void * arg4, void * arg5);
    
    virtual void setCountedActionLimit(UInt32 limit);
    virtual bool allDoneProcessing();
    virtual void setEnqueueEnable(bool enabled);
    virtual void removeAllEntries();
    virtual void disable() APPLE_KEXT_OVERRIDE;
    virtual void disableWL();
    
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 0);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 1);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 2);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 3);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 4);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 5);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 6);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 7);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 8);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 9);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 10);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 11);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 12);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 13);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 14);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 15);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 16);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 17);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 18);
    OSMetaClassDeclareReservedUnused(IOWorkQueue, 19);
    
protected:    
    IOWorkQueueCall *           mWorkQueueHeadCall; //72
    IOWorkQueueCall *           mWorkQueueTailCall; //80
    IOSimpleLock *              mWorkQueueCallSimpleLock; //88
    
    struct ExpansionData
    {
        IOWorkLoop *            eWorkLoop; //0
        IOCommandGate *         eCommandGate; //8
        bool                    eStop; //16
        IOThread                eThread[20]; //24
        uint8_t                 eThreadLimit[20]; //184
    };
    ExpansionData *             mExpansionData; //96
    
#define eWorkLoop               IOWorkQueue::mExpansionData->eWorkLoop
#define eCommandGate            IOWorkQueue::mExpansionData->eCommandGate
#define eStop                   IOWorkQueue::mExpansionData->eStop
#define eThread                 IOWorkQueue::mExpansionData->eThread
#define eThreadLimit            IOWorkQueue::mExpansionData->eThreadLimit
    
    void *                      mUnusedPointer; //104
    
    IOWorkQueueOperationMode    mOperationMode; //112
    SInt32                      mThreadCounter; //116
    UInt32                      mCountedActionLimit; //120
    bool                        mDebug; //124
    bool                        mEnqueueEnabled; //125
    char                        mWorkQueueName[0x64]; //126
};

#endif
