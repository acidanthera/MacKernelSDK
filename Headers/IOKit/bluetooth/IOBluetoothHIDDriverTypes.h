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
/*!
 *   @header
 *   This header contains enumerations for IOBluetoothHIDDriver.
 */

#pragma once

// Vendor ID Sources
enum
{
	kVendorIDSourceBluetoothSIG					= 0x1,
	kVendorIDSourceUSBIF						= 0x2
};

// Bluetooth HID Transaction Headers
enum
{
	IOBTHID_HANDSHAKE_HEADER					= 0x00,
	IOBTHID_HID_CONTROL_HEADER					= 0x10,
	IOBTHID_GET_REPORT_HEADER					= 0x40,
	IOBTHID_SET_REPORT_HEADER					= 0x50,
	IOBTHID_GET_PROTOCOL_HEADER					= 0x60,
	IOBTHID_SET_PROTOCOL_HEADER					= 0x70,
	IOBTHID_GET_IDLE_HEADER						= 0x80,
	IOBTHID_SET_IDLE_HEADER						= 0x90,
	IOBTHID_DATA_HEADER							= 0xA0,
	IOBTHID_DATC_HEADER							= 0xB0
};

// Handshake Types
enum
{
	IOBTHID_HANDSHAKE_SUCCESSFUL				= 0x0,
	IOBTHID_HANDSHAKE_NOT_READY					= 0x1,
	IOBTHID_HANDSHAKE_INVALID_REPORT_ID			= 0x2,
	IOBTHID_HANDSHAKE_UNSUPPORTED_REQUEST		= 0x3,
	IOBTHID_HANDSHAKE_INVALID_PARAMETER			= 0x4,
	IOBTHID_HANDSHAKE_ERR_UNKNOWN				= 0xD,
	IOBTHID_HANDSHAKE_ERR_FATAL					= 0xF
};

// HID_Control Types
enum
{
	IOBTHID_CONTROL_NOP							= 0x0,
	IOBTHID_CONTROL_HARD_RESET					= 0x1,
	IOBTHID_CONTROL_SOFT_RESET					= 0x2,
	IOBTHID_CONTROL_SUSPEND						= 0x3,
	IOBTHID_CONTROL_EXIT_SUSPEND				= 0x4,
	IOBTHID_CONTROL_VC_UNPLUG					= 0x5
};

// Protocol Types
enum
{
	IOBTHID_BOOT_PROTOCOL						= 0x0,
	IOBTHID_REPORT_PROTOCOL						= 0x1
};

// Report Types
enum
{
	IOBTHID_RESERVED_REPORT						= 0x0,
	IOBTHID_OTHER_REPORT						= 0x0,
	IOBTHID_INPUT_REPORT						= 0x1,
	IOBTHID_OUTPUT_REPORT						= 0x2,
	IOBTHID_FEATURE_REPORT						= 0x3
};