/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
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

#ifndef _IOKIT_IOREPORTUSERCLIENT_H
#define _IOKIT_IOREPORTUSERCLIENT_H

#include <IOKit/IOUserClient.h>
#include <IOKit/IOReportHub.h>

class IOReportUserClient : public IOUserClient
{
    OSDeclareDefaultStructors(IOReportUserClient)
    
public:
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_5
    virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments * arguments, IOExternalMethodDispatch * dispatch = NULL, OSObject * target = NULL, void * reference = NULL) APPLE_KEXT_OVERRIDE;
#endif
    
    virtual bool init( OSDictionary * dictionary ) APPLE_KEXT_OVERRIDE;
    virtual bool initWithTask( task_t owningTask, void * securityToken, UInt32 type ) APPLE_KEXT_OVERRIDE;
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual bool stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn clientClose() APPLE_KEXT_OVERRIDE;
    virtual IOService * getService() APPLE_KEXT_OVERRIDE;
    virtual IOReturn clientMemoryForType( UInt32 type, IOOptionBits * options, IOMemoryDescriptor ** memory ) APPLE_KEXT_OVERRIDE;
    
    virtual IOReturn open( IOOptionBits options );
    virtual IOReturn close();
    
    static IOReturn _open( IOReportUserClient *, void *, IOExternalMethodArguments * );
    static IOReturn _close( IOReportUserClient *, void *, IOExternalMethodArguments * );
    static IOReturn goto_configureInterests( IOReportUserClient *, void *, IOExternalMethodArguments * );
    static IOReturn goto_getValues( IOReportUserClient *, void *, IOExternalMethodArguments * );
    IOReturn _configureInterests( IOReportInterestList *, IOByteCount * );
    IOReturn _getInterests( IOByteCount, OSDictionary **, IOBufferMemoryDescriptor ** );
    IOReturn _getValues( IOByteCount );

protected:
    IOOptionBits mHubOptions;
    task_t mOwningTask;
    IOReportHub * mHub;
    IOLock * mHubLock;
    OSArray * mReportDictionaryArray;
    OSArray * mSubscriptionBufferArray;
};

#endif
