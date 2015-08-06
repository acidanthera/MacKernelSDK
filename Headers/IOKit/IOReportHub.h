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

#ifndef _IOKIT_IOREPORTHUB_H
#define _IOKIT_IOREPORTHUB_H

#include <IOKit/IOService.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

extern IOReturn _getInterests(OSDictionary *, OSArray **, OSArray **, OSArray **);
extern IOReturn _copyServiceGetChannels(OSArray *, OSArray *, OSArray *, UInt32, IOService **, IOReportChannelList **, OSNumber **);

class IOReportHub : public IOService
{
    OSDeclareDefaultStructors(IOReportHub)
    
public:
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual IOReturn getReportBufferSize( OSDictionary * dict, int * size );
    virtual IOReturn getValues( OSDictionary * dict, IOBufferMemoryDescriptor * buffer );
    IOReturn callConfigureReport(OSDictionary *, UInt32, int *);
};

#endif
