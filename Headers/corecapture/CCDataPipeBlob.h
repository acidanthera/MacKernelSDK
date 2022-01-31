/*
 * Released under "The BSD 3-Clause License"
 *
 * Copyright (c) 2021 cjiang. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The names of its contributors may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _CCDATAPIPEBLOB_H
#define _CCDATAPIPEBLOB_H

class CCDataPipeBlob : public OSObject
{
    // default
}; // 88

#endif

/*
CCDataPipeBlob::copyBufferToUser(ulong long,ulong,ulong)    __text    000000000000529E    00000063    00000028    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlob::free(void)    __text    0000000000005230    0000005D    00000010    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlob::getBuffer(void)    __text    000000000000528E    00000008    00000008    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlob::getBufferLength(void)    __text    0000000000005296    00000008    00000008    00000000    R    .    .    .    .    B    T    .

CCDataPipeBlobOD::MetaClass::alloc(void)    __text    0000000000005502    00000040    00000018    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOD::free(void)    __text    0000000000005486    0000002E    00000010    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOD::getBuffer(void)    __text    00000000000054B4    0000001B    00000008    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOD::getBufferLength(void)    __text    00000000000054D0    0000001E    00000008    00000000    R    .    .    .    .    B    T    .

CCDataPipeBlobOS::MetaClass::alloc(void)    __text    000000000000540C    00000040    00000018    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOS::free(void)    __text    0000000000005390    0000002E    00000010    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOS::getBuffer(void)    __text    00000000000053BE    0000001B    00000008    00000000    R    .    .    .    .    B    T    .
CCDataPipeBlobOS::getBufferLength(void)    __text    00000000000053DA    0000001E    00000008    00000000    R    .    .    .    .    B    T    .
*/
