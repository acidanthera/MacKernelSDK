/*
 * Copyright (c) 2019 Apple Inc. All rights reserved.
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

#ifndef _SKYWALK_OS_CHANNEL_EVENT_H_
#define _SKYWALK_OS_CHANNEL_EVENT_H_

#ifdef PRIVATE
#include <stdint.h>
#include <mach/vm_types.h>

typedef enum {
	CHANNEL_EVENT_PACKET_TRANSMIT_STATUS = 1,
#if defined(LIBSYSCALL_INTERFACE) || defined(BSD_KERNEL_PRIVATE)
	CHANNEL_EVENT_MIN    = CHANNEL_EVENT_PACKET_TRANSMIT_STATUS,
	CHANNEL_EVENT_MAX    = CHANNEL_EVENT_PACKET_TRANSMIT_STATUS,
#endif /* LIBSYSCALL_INTERFACE || BSD_KERNEL_PRIVATE */
} os_channel_event_type_t;

typedef enum {
	CHANNEL_EVENT_SUCCESS = 0,
	CHANNEL_EVENT_PKT_TRANSMIT_STATUS_ERR_FLUSH = 1,
	CHANNEL_EVENT_PKT_TRANSMIT_STATUS_ERR_RETRY_FAILED = 2,
} os_channel_event_error_t;

typedef struct os_channel_event_packet_transmit_status {
	packet_id_t    packet_id;
	int32_t        packet_status;
} os_channel_event_packet_transmit_status_t;

#ifndef KERNEL
/*
 * opaque handles
 */
typedef uint64_t             os_channel_event_handle_t;
typedef mach_vm_address_t    os_channel_event_t;

struct os_channel_event_data {
	os_channel_event_type_t    event_type;
	boolean_t                  event_more;
	uint16_t                   event_data_length;
	uint8_t                    *event_data;
};

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
extern int
os_channel_event_get_next_event(const os_channel_event_handle_t event_handle,
    const os_channel_event_t prev_event, os_channel_event_t *event);
extern int os_channel_event_get_event_data(const os_channel_event_t, struct os_channel_event_data *);
#endif  /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#endif /* KERNEL */

#if defined(LIBSYSCALL_INTERFACE) || defined(BSD_KERNEL_PRIVATE)
/*
 * The metadata object is placed at the front of every batch of events.
 */
struct __kern_channel_event_metadata {
	os_channel_event_type_t    emd_etype;
	uint32_t                   emd_nevents;
};
#define __KERN_CHANNEL_EVENT_OFFSET    \
    (sizeof(struct __kern_channel_event_metadata))

struct __kern_channel_event {
	os_channel_event_type_t    ev_type;
	uint32_t                   ev_flags;
	uint16_t                   _reserved;
	uint16_t                   ev_dlen;
	uint8_t                    ev_data[];
};

/* event_flags */
#define CHANNEL_EVENT_FLAG_MORE_EVENT    0x1

/* convenience macro */
#define CHANNEL_EVENT_TX_STATUS_LEN    (sizeof(struct __kern_channel_event) + \
    sizeof(os_channel_event_packet_transmit_status_t))
#endif /* LIBSYSCALL_INTERFACE || BSD_KERNEL_PRIVATE */

#if defined(BSD_KERNEL_PRIVATE)
__BEGIN_DECLS
extern errno_t kern_channel_event_transmit_status(const kern_packet_t,
    const ifnet_t);
extern void kern_channel_event_notify(struct __kern_channel_ring *);
extern int kern_channel_event_sync(struct __kern_channel_ring *, struct proc *,
    uint32_t);
__END_DECLS
#endif /* BSD_KERNEL_PRIVATE */

#endif /* PRIVATE */
#endif /* !_SKYWALK_OS_CHANNEL_EVENT_H_ */
