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
/*!
 *   @header IOBluetoothACPIMethods.h
 *   This header contains the definition of the IOBluetoothACPIMethods class.
 */

#ifndef _IOKIT_IOBLUETOOTHACPIMETHODS_H
#define _IOKIT_IOBLUETOOTHACPIMETHODS_H

#include <IOKit/IOService.h>
#include <os/log.h>

class IOBluetoothHCIController;
class IOACPIPlatformDevice;

class IOBluetoothACPIMethods : public IOService
{
    OSDeclareDefaultStructors(IOBluetoothACPIMethods);
    
    friend class IOBluetoothHCIController;
    friend class IOBluetoothHostControllerTransport;
    
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
