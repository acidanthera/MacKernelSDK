/*
 *  Released under "The GNU General Public License (GPL-2.0)"
 *
 *  Copyright (c) 2021 cjiang. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
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
        Action action; //0
        void * arg0; //8
        void * arg1; //16
        void * arg2; //24
        void * arg3; //32
        void * arg4; //40
        void * arg5; //48
        
        IOWorkQueueCall * nextCall; //56
        IOWorkQueueCall * previousCall; //64
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
