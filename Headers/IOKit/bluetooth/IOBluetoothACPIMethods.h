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
/*!
 *   @header IOBluetoothACPIMethods.h
 *   This header contains the definition of the IOBluetoothACPIMethods class.
 */

#ifndef _IOBLUETOOTH_ACPIMETHODS_H
#define _IOBLUETOOTH_ACPIMETHODS_H

#include <IOKit/IOService.h>
#include <os/log.h>

class IOBluetoothHCIController;
class IOACPIPlatformDevice;

class IOBluetoothACPIMethods : public IOService
{
    OSDeclareDefaultStructors(IOBluetoothACPIMethods)

public:
    bool init( IOService * provider );
    virtual void free() APPLE_KEXT_OVERRIDE;
    virtual bool start( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void stop( IOService * provider ) APPLE_KEXT_OVERRIDE;
    virtual void SetBluetoothFamily( IOBluetoothHCIController * controller );
    virtual IOReturn CheckSpecialGPIO();
    virtual IOReturn SetBTRS();
    virtual IOReturn SetBTPU();
    virtual IOReturn SetBTPD();
    virtual IOReturn SetBTRB(bool, bool *);
    virtual IOReturn SetBTLP(bool);
    virtual bool ResetMethodIsAvailable();
    virtual bool ROMBootMethodIsAvailable();
    virtual bool PowerUpMethodIsAvailable();
    virtual bool PowerDownMethodIsAvailable();
    virtual bool LowPowerMethodIsAvailable();
    virtual UInt32 GetCurrentTime( void );
    virtual int ConvertErrorCodeToString(int, char *, char *);

protected:
    IOBluetoothHCIController * mBluetoothFamily; //136
    IOACPIPlatformDevice * mACPIDevice; //144
    IOService * mProvider; //152
    bool mResetMethodAvailable; //160 BTRS
    bool mROMBootMethodAvailable; //161 BTRB
    bool mPowerUpMethodAvailable; //162 BTPU
    bool mPowerDownMethodAvailable; //163 BTPD
    bool mLowPowerMethodAvailable; //164 BTLP
    os_log_t mInternalOSLogObject; //168
};

#endif
