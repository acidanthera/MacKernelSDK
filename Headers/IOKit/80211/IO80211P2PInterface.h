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

#ifndef _IO80211P2PINTERFACE_H
#define _IO80211P2PINTERFACE_H

#include <IOKit/80211/IO80211VirtualInterface.h>

class IO80211P2PInterface : public IO80211VirtualInterface
{
    OSDeclareDefaultStructors( IO80211P2PInterface )

public:
    virtual errno_t configureIfnet() APPLE_KEXT_OVERRIDE;
    bool isAPSTA();
    errno_t apStaConfigureIfnet();
    bool isP2P();
    errno_t p2pConfigureIfnet();
    
    virtual bool createPeerManager( ether_addr * inAddress, IO80211PeerManager ** outManager ) APPLE_KEXT_OVERRIDE;
    bool apStaCreatePeerManager( ether_addr * inAddress, IO80211PeerManager ** outManager );
    bool p2pCreatePeerManager( ether_addr * inAddress, IO80211PeerManager ** outManager );
    
    virtual bool attachToBpf() APPLE_KEXT_OVERRIDE;
    bool apStaAttachToBpf();
    bool p2pAttachToBpf();
    
    virtual void setUnitNumber( const char * name ) APPLE_KEXT_OVERRIDE;
    void apStaSetUnitNumber( const char * name );
    void p2pSetUnitNumber( const char * name );
    
    virtual void initIfnetEparams( struct ifnet_init_eparams * eparams ) APPLE_KEXT_OVERRIDE;
    void apStaInitIfnetEparams( struct ifnet_init_eparams * eparams );
    errno_t apsta_if_output_pre_enqueue( ifnet_t interface, mbuf_t data );
    
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  0 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  1 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  2 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  3 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  4 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  5 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  6 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  7 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  8 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface,  9 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 10 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 11 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 12 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 13 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 14 );
    OSMetaClassDeclareReservedUnused( IO80211P2PInterface, 15 );
};

#endif
