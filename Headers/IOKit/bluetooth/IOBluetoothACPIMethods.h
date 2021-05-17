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

#ifndef _IOKIT_BLUETOOTH_IOBLUETOOTHACPIMETHODS_H
#define _IOKIT_BLUETOOTH_IOBLUETOOTHACPIMETHODS_H

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
    
/*! @function SetBluetoothFamily
 *   @abstract Sets the mBluetoothFamily member variable.
 *   @param controller mBluetoothFamily is set to this IOBluetoothHCIController instance. */
    
    virtual void SetBluetoothFamily( IOBluetoothHCIController * controller );
    
    virtual void CheckSpecialGPIO();
    virtual IOReturn SetBTRS();
    virtual IOReturn SetBTPU();
    virtual IOReturn SetBTPD();
    virtual IOReturn SetBTRB(bool, bool *);
    virtual IOReturn SetBTLP(bool);
    
/*! @function ResetMethodIsAvailable
 *   @abstract Determines if the Bluetooth ACPI reset method (BTRS) is available.
 *   @discussion This function is implemented by returning mResetMethodAvailable.
 *   @result mResetMethodAvailable is returned. */
    
    virtual bool ResetMethodIsAvailable();
    
/*! @function ROMBootMethodIsAvailable
 *   @abstract Determines if the Bluetooth ACPI ROM boot method (BTRB) is available.
 *   @discussion This function is implemented by returning mROMBootMethodAvailable.
 *   @result mROMBootMethodAvailable is returned. */
    
    virtual bool ROMBootMethodIsAvailable();
    
/*! @function PowerUpMethodIsAvailable
 *   @abstract Determines if the Bluetooth ACPI power up method (BTPU) is available.
 *   @discussion This function is implemented by returning mPowerUpMethodAvailable.
 *   @result mPowerUpMethodAvailable is returned. */
    
    virtual bool PowerUpMethodIsAvailable();
    
/*! @function PowerUpMethodIsAvailable
 *   @abstract Determines if the Bluetooth ACPI power down method (BTPD) is available.
 *   @discussion This function is implemented by returning mPowerDownMethodAvailable.
 *   @result mPowerDownMethodAvailable is returned. */
    
    virtual bool PowerDownMethodIsAvailable();
    
/*! @function LowPowerMethodIsAvailable
 *   @abstract Determines if the Bluetooth ACPI low power method (BTLP) is available.
 *   @discussion This function is implemented by returning mLowPowerMethodAvailable.
 *   @result mLowPowerMethodAvailable is returned. */
    
    virtual bool LowPowerMethodIsAvailable();
    
    virtual UInt32 GetCurrentTime();
    
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
