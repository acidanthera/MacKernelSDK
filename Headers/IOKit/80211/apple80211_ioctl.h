/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _APPLE80211_IOCTL_H_
#define _APPLE80211_IOCTL_H_

#include <Availability.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <sys/param.h>
#include <sys/ioctl.h>

#include <IOKit/80211/apple80211_var.h>

struct apple80211req
{
    char        req_if_name[IFNAMSIZ];
    int         req_type;
    int         req_val;
    u_int32_t   req_len;
    void        *req_data;
    u_int32_t   req_ptr_len;
};

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_15
#define SIOCSA80211 2150656456
#define SIOCGA80211 3224398281
#else
#define SIOCSA80211 2150132168
#define SIOCGA80211 3223873993
#endif

#define APPLE80211_AWDL_CAP_CCA_STATS   2
#define APPLE80211_AWDL_CAP_SEC_PAYLOAD 0x100000000

// req_type

#define APPLE80211_IOC_SSID                                                  1

#define APPLE80211_IOC_AUTH_TYPE                                             2
#define     APPLE80211_AUTH_TYPE_UNICAST                                     1    // req_val, SIOCGA80211 only
#define     APPLE80211_AUTH_TYPE_MULTICAST                                   2    // req_val, SIOCGA80211 only

#define APPLE80211_IOC_CIPHER_KEY                                            3
#define     APPLE80211_CIPHER_KEY_UNICAST                                    1    // req_val
#define     APPLE80211_CIPHER_KEY_MULICAST                                   2    // req_val

#define APPLE80211_IOC_CHANNEL                                               4

#define APPLE80211_IOC_POWERSAVE                                             5

#define APPLE80211_IOC_PROTMODE                                              6

#define APPLE80211_IOC_TXPOWER                                               7
#define APPLE80211_IOC_RATE                                                  8
#define APPLE80211_IOC_BSSID                                                 9

#define APPLE80211_IOC_SCAN_REQ                                             10

#define APPLE80211_IOC_SCAN_RESULT                                          11

#define APPLE80211_IOC_CARD_CAPABILITIES                                    12

#define APPLE80211_IOC_STATE                                                13
#define APPLE80211_IOC_PHY_MODE                                             14

#define APPLE80211_IOC_OP_MODE                                              15
#define APPLE80211_IOC_RSSI                                                 16
#define APPLE80211_IOC_NOISE                                                17

#define APPLE80211_IOC_INT_MIT                                              18
#define     APPLE80211_IOC_INT_MIT_OFF                                       1    // req_val
#define     APPLE80211_IOC_INT_MIT_ON                                        2    // req_val

// card power
#define APPLE80211_IOC_POWER                                                19

#define APPLE80211_IOC_ASSOCIATE                                            20
#define APPLE80211_IOC_ASSOCIATE_RESULT                                     21
#define APPLE80211_IOC_DISASSOCIATE                                         22
#define APPLE80211_IOC_STATUS_DEV_NAME                                      23

#define APPLE80211_IOC_IBSS_MODE                                            24
#define     APPLE80211_IOC_IBSS_MODE_START                                   1    // req_val
#define     APPLE80211_IOC_IBSS_MODE_STOP                                    2    // req_val

#define APPLE80211_IOC_HOST_AP_MODE                                         25
#define     APPLE80211_IOC_HOST_AP_MODE_START                                1    // req_val
#define     APPLE80211_IOC_HOST_AP_MODE_STOP                                 2    // req_val

#define APPLE80211_IOC_AP_MODE                                              26
#define APPLE80211_IOC_SUPPORTED_CHANNELS                                   27
#define APPLE80211_IOC_LOCALE                                               28
#define APPLE80211_IOC_DEAUTH                                               29
#define APPLE80211_IOC_COUNTERMEASURES                                      30
#define APPLE80211_IOC_FRAG_THRESHOLD                                       31
#define APPLE80211_IOC_RATE_SET                                             32
#define APPLE80211_IOC_SHORT_SLOT                                           33
#define APPLE80211_IOC_MULTICAST_RATE                                       34
#define APPLE80211_IOC_SHORT_RETRY_LIMIT                                    35
#define APPLE80211_IOC_LONG_RETRY_LIMIT                                     36
#define APPLE80211_IOC_TX_ANTENNA                                           37
#define APPLE80211_IOC_RX_ANTENNA                                           38
#define APPLE80211_IOC_ANTENNA_DIVERSITY                                    39
#define APPLE80211_IOC_ROM                                                  40
#define APPLE80211_IOC_DTIM_INT                                             41
#define APPLE80211_IOC_STATION_LIST                                         42
#define APPLE80211_IOC_DRIVER_VERSION                                       43
#define APPLE80211_IOC_HARDWARE_VERSION                                     44
#define APPLE80211_IOC_RAND                                                 45
#define APPLE80211_IOC_RSN_IE                                               46
#define APPLE80211_IOC_BACKGROUND_SCAN                                      47
#define APPLE80211_IOC_AP_IE_LIST                                           48
#define APPLE80211_IOC_STATS                                                49
#define APPLE80211_IOC_ASSOCIATION_STATUS                                   50
#define APPLE80211_IOC_COUNTRY_CODE                                         51
#define APPLE80211_IOC_DEBUG_FLAGS                                          52
#define APPLE80211_IOC_LAST_RX_PKT_DATA                                     53
#define APPLE80211_IOC_RADIO_INFO                                           54
#define APPLE80211_IOC_GUARD_INTERVAL                                       55
#define APPLE80211_IOC_MIMO_POWERSAVE                                       56
#define APPLE80211_IOC_MCS                                                  57
#define APPLE80211_IOC_RIFS                                                 58
#define APPLE80211_IOC_LDPC                                                 59
#define APPLE80211_IOC_MSDU                                                 60
#define APPLE80211_IOC_MPDU                                                 61
#define APPLE80211_IOC_BLOCK_ACK                                            62
#define APPLE80211_IOC_PLS                                                  63
#define APPLE80211_IOC_PSMP                                                 64
#define APPLE80211_IOC_PHY_SUB_MODE                                         65
#define APPLE80211_IOC_MCS_INDEX_SET                                        66
#define APPLE80211_IOC_CACHE_THRESH_BCAST                                   67
#define APPLE80211_IOC_CACHE_THRESH_DIRECT                                  68
#define APPLE80211_IOC_WOW_PARAMETERS                                       69
#define APPLE80211_IOC_WOW_ENABLED                                          70
#define APPLE80211_IOC_40MHZ_INTOLERANT                                     71
#define APPLE80211_IOC_PID_LOCK                                             72
#define APPLE80211_IOC_STA_IE_LIST                                          73
#define APPLE80211_IOC_STA_AUTHORIZE                                        74
#define APPLE80211_IOC_STA_DISASSOCIATE                                     75
#define APPLE80211_IOC_STA_DEAUTH                                           76
#define APPLE80211_IOC_RSN_CONF                                             77
#define APPLE80211_IOC_KEY_RSC                                              78
#define APPLE80211_IOC_STA_STATS                                            79
#define APPLE80211_IOC_ROAM_THRESH                                          80
#define APPLE80211_IOC_VENDOR_DBG_FLAGS                                     81
#define APPLE80211_IOC_CACHE_AGE_THRESH                                     82
#define APPLE80211_IOC_PMK_CACHE                                            83
#define APPLE80211_IOC_LINK_QUAL_EVENT_PARAMS                               84
#define APPLE80211_IOC_IE                                                   85
#define APPLE80211_IOC_SCAN_REQ_MULTIPLE                                    86
#define APPLE80211_IOC_BTCOEX_MODE                                          87
#define APPLE80211_IOC_WOW_TEST                                             88
#define APPLE80211_IOC_CLEAR_PMK_CACHE                                      89
#define APPLE80211_IOC_SCANCACHE_CLEAR                                      90
#define APPLE80211_IOC_P2P_ENABLE                                           91
#define APPLE80211_IOC_P2P_LISTEN                                           92
#define APPLE80211_IOC_P2P_SCAN                                             93
#define APPLE80211_IOC_VIRTUAL_IF_CREATE                                    94
#define APPLE80211_IOC_VIRTUAL_IF_DELETE                                    95
#define APPLE80211_IOC_VIRTUAL_IF_ROLE                                      96
#define APPLE80211_IOC_VIRTUAL_IF_PARENT                                    97
#define APPLE80211_IOC_P2P_GO_CONF                                          98
#define APPLE80211_IOC_P2P_NOA_LIST                                         99
#define APPLE80211_IOC_P2P_OPP_PS                                          100
#define APPLE80211_IOC_P2P_CT_WINDOW                                       101
#define APPLE80211_IOC_BT_COEX_FLAGS                                       102
#define APPLE80211_IOC_CURRENT_NETWORK                                     103
#define APPLE80211_IOC_BT_POWER                                            104
#define APPLE80211_IOC_AVAILABILITY                                        105
#define APPLE80211_IOC_RSSI_BOUNDS                                         106
#define APPLE80211_IOC_ROAM                                                107
#define APPLE80211_IOC_TX_CHAIN_POWER                                      108
#define APPLE80211_IOC_CDD_MODE                                            109
#define APPLE80211_IOC_LAST_BCAST_SCAN_TIME                                110
#define APPLE80211_IOC_THERMAL_THROTTLING                                  111
#define APPLE80211_IOC_FACTORY_MODE                                        112
#define APPLE80211_IOC_REASSOCIATE                                         113

#define APPLE80211_IOC_POWER_DEBUG_INFO                                    115
#define APPLE80211_IOC_AWDL_SYNC_PARAMS                                    116
#define APPLE80211_IOC_AWDL_SYNC_ENABLED                                   117
#define APPLE80211_IOC_AWDL_EXTENSION_STATE_MACHINE_PARAMETERS             118
#define APPLE80211_IOC_AWDL_SERVICE_PARAMS                                 119
#define APPLE80211_IOC_AWDL_PEER_SERVICE_REQUEST                           120
#define APPLE80211_IOC_AWDL_ELECTION_ALGORITHM_ENABLED                     121
#define APPLE80211_IOC_AWDL_ELECTION_ID                                    122
#define APPLE80211_IOC_AWDL_MAX_TREE_DEPTH                                 123
#define APPLE80211_IOC_AWDL_GUARD_TIME                                     124
#define APPLE80211_IOC_AWDL_BSSID                                          125
#define APPLE80211_IOC_AWDL_ELECTION_METRIC                                126
#define APPLE80211_IOC_AWDL_AVAILABILITY_WINDOW_AP_ALIGNMENT               127
#define APPLE80211_IOC_AWDL_SYNC_FRAME_AP_BEACON_ALIGNMENT                 128
#define APPLE80211_IOC_AWDL_SYNCHRONIZATION_CHANNEL_SEQUENCE               129
#define APPLE80211_IOC_PEER_CACHE_MAXIMUM_SIZE                             130
#define APPLE80211_IOC_AWDL_OUI                                            131
#define APPLE80211_IOC_AWDL_MASTER_CHANNEL                                 132
#define APPLE80211_IOC_AWDL_TOP_MASTER                                     133
#define APPLE80211_IOC_AWDL_SYNC_STATE                                     134
#define APPLE80211_IOC_AWDL_ELECTION_RSSI_THRESHOLDS                       135
#define APPLE80211_IOC_AWDL_PRESENCE_MODE                                  136
#define APPLE80211_IOC_AWDL_ELECTION_MASTER_COUNTS                         137
#define APPLE80211_IOC_AWDL_PERIODIC_SYNC_FRAME_PACKET_LIFETIME            138
#define APPLE80211_IOC_AWDL_MASTER_MODE_SYNC_FRAME_PERIOD                  139
#define APPLE80211_IOC_AWDL_NON_ELECTION_MASTER_MODE_SYNC_FRAME_PERIOD     140
#define APPLE80211_IOC_AWDL_EXPLICIT_AVAILABILITY_WINDOW_EXTENSION_OPT_OUT 141
#define APPLE80211_IOC_AWDL_GET_AWDL_MASTER_DATABASE                       142
#define APPLE80211_IOC_PEER_CACHE_CONTROL                                  143
#define APPLE80211_IOC_AWDL_BATTERY_LEVEL                                  144
#define APPLE80211_IOC_AWDL_BT_COEX_AW_PROTECTED_PERIOD_LENGTH             145
#define APPLE80211_IOC_AWDL_BT_COEX_AGREEMENT                              146
#define APPLE80211_IOC_AWDL_BT_COEX_AGREEMENT_ENABLED                      147
#define APPLE80211_IOC_AWDL_STRATEGY                                       148
#define APPLE80211_IOC_AWDL_OOB_REQUEST                                    149
#define APPLE80211_IOC_AWDL_MAX_NO_MASTER_PERIODS                          150
#define APPLE80211_IOC_AWDL_SYNC_FRAME_TEMPLATE                            151
#define APPLE80211_IOC_LOG_FLAGS                                           152
#define APPLE80211_IOC_PEER_STATS                                          153
#define APPLE80211_IOC_HT_CAPABILITY                                       154
#define APPLE80211_IOC_AWDL_ELECTION_PARAMS                                155
#define APPLE80211_IOC_LINK_CHANGED_EVENT_DATA                             156
#define APPLE80211_IOC_GET_DEBUG_INFO                                      157
#define APPLE80211_IOC_AWDL_DEVICE_CAPABILITIES                            158
#define APPLE80211_IOC_AWDL_RSSI_MEASUREMENT_REQUEST                       159
#define APPLE80211_IOC_AWDL_AES_KEY                                        160
#define APPLE80211_IOC_AWDL_SCAN_RESERVED_TIME                             161
#define APPLE80211_IOC_AWDL_CTL                                            162
#define APPLE80211_IOC_AWDL_SOCIAL_TIME_SLOTS                              163
#define APPLE80211_IOC_AWDL_PEER_TRAFFIC_REGISTRATION                      164
#define APPLE80211_IOC_EXTENDED_STATS                                      165
#define APPLE80211_IOC_BEACON_PERIOD                                       166
#define APPLE80211_IOC_AWDL_FORCED_ROAM_CONFIG                             167
#define APPLE80211_IOC_AWDL_QUIET                                          168
#define APPLE80211_IOC_ACL_POLICY                                          169
#define APPLE80211_IOC_ACL_ADD                                             170
#define APPLE80211_IOC_ACL_REMOVE                                          171
#define APPLE80211_IOC_ACL_FLUSH                                           172
#define APPLE80211_IOC_ACL_LIST                                            173
#define APPLE80211_IOC_CHAIN_ACK                                           174
#define APPLE80211_IOC_DESENSE                                             175
#define APPLE80211_IOC_OFFLOAD_SCANNING                                    176
#define APPLE80211_IOC_OFFLOAD_RSN                                         177
#define APPLE80211_IOC_OFFLOAD_COUNTRY_CODE                                178
#define APPLE80211_IOC_OFFLOAD_KEEPALIVE_L2                                179
#define APPLE80211_IOC_OFFLOAD_ARP_NDP                                     180
#define APPLE80211_IOC_VHT_MCS_INDEX_SET                                   181
#define APPLE80211_IOC_DWDS                                                182
#define APPLE80211_IOC_INTERRUPT_STATS                                     183
#define APPLE80211_IOC_INTERRUPT_STATS_RESET                               184
#define APPLE80211_IOC_TIMER_STATS                                         185
#define APPLE80211_IOC_TIMER_STATS_RESET                                   186
#define APPLE80211_IOC_OFFLOAD_STATS                                       187
#define APPLE80211_IOC_OFFLOAD_STATS_RESET                                 188
#define APPLE80211_IOC_OFFLOAD_BEACONS                                     189
#define APPLE80211_IOC_ROAMING                                             190
#define APPLE80211_IOC_OFFLOAD_ARP                                         191
#define APPLE80211_IOC_OFFLOAD_NDP                                         192
#define APPLE80211_IOC_OFFLOAD_SCAN                                        193
#define APPLE80211_IOC_DESENSE_LEVEL                                       194
#define APPLE80211_IOC_MCS_VHT                                             195
#define APPLE80211_IOC_TX_NSS                                              196
#define APPLE80211_IOC_GAS_REQ                                             197
#define APPLE80211_IOC_GAS_START                                           198
#define APPLE80211_IOC_GAS_SET_PEER                                        199
#define APPLE80211_IOC_GAS_RESULTS                                         200
#define APPLE80211_IOC_AWDL_BTLE_PEER_INDICATION                           201
#define APPLE80211_IOC_AWDL_BTLE_STATE_PARAMS                              202
#define APPLE80211_IOC_AWDL_PEER_DATABASE                                  203
#define APPLE80211_IOC_AWDL_BTLE_ENABLE_SYNC_WITH_PARAMS                   204
#define APPLE80211_IOC_AWDL_SECONDARY_MASTER_CHANNEL                       205
#define APPLE80211_IOC_PHY_STATS                                           206
#define APPLE80211_IOC_CHANNELS_INFO                                       207
#define APPLE80211_IOC_AWDL_AF_TX_MODE                                     208
#define APPLE80211_IOC_ERROR_STRING                                        209
#define APPLE80211_IOC_ERROR_NO                                            210
#define APPLE80211_IOC_AWDL_PIGGYBACK_SCAN_REQ                             211
#define APPLE80211_IOC_AWDL_PRIVATE_ELECTION_ID                            212
#define APPLE80211_IOC_AWDL_MIN_RATE                                       213
#define APPLE80211_IOC_VHT_CAPABILITY                                      214
#define APPLE80211_IOC_BGSCAN_CACHE_RESULTS                                215
#define APPLE80211_IOC_ROAM_PROFILE                                        216
#define APPLE80211_IOC_AWDL_OPER_MODE                                      217
#define APPLE80211_IOC_RESTORE_DEFAULTS                                    218
#define APPLE80211_IOC_AWDL_ENCRYPTION_KEYS                                219
#define APPLE80211_IOC_AWDL_ENCRYPTION_TYPE                                220
#define APPLE80211_IOC_BTCOEX_PROFILES                                     221
#define APPLE80211_IOC_BTCOEX_CONFIG                                       222
#define APPLE80211_IOC_AWDL_STATISTICS                                     223
#define APPLE80211_IOC_AWDL_ENABLE_ROAMING                                 224
#define APPLE80211_IOC_AWDL_OOB_AUTO_REQUEST                               225
#define APPLE80211_IOC_AWDL_TXCAL_PERIOD                                   226
#define APPLE80211_IOC_CHIP_COUNTER_STATS                                  227
#define APPLE80211_IOC_DBG_GUARD_TIME_PARAMS                               228
#define APPLE80211_IOC_AWDL_AWDL_ADVERTISERS                               229
#define APPLE80211_IOC_LEAKY_AP_STATS_MODE                                 230
#define APPLE80211_IOC_CAPTURE                                             231
#define APPLE80211_IOC_LEAKY_AP_STATS                                      232
#define APPLE80211_IOC_AWDL_BLOCK_SET_COMMANDS                             233
#define APPLE80211_IOC_LEAKY_AP_AWD_MODE                                   234
#define APPLE80211_IOC_BTCOEX_OPTIONS                                      235
#define APPLE80211_IOC_FORCE_SYNC_TO_PEER                                  236
#define APPLE80211_IOC_COUNTRY_CHANNELS                                    237
#define APPLE80211_IOC_PRIVATE_MAC                                         238
#define APPLE80211_IOC_RESET_CHIP                                          239
#define APPLE80211_IOC_CRASH                                               240
#define APPLE80211_IOC_RANGING_ENABLE                                      241
#define APPLE80211_IOC_RANGING_START                                       242
#define APPLE80211_IOC_RANGING_AUTHENTICATE                                243
#define APPLE80211_IOC_AWDL_PREFERRED_CHANNELS                             244
#define APPLE80211_IOC_LEAKY_AP_SSID_STATS                                 245
#define APPLE80211_IOC_AWDL_RSDB_CAPS                                      246
#define APPLE80211_IOC_AWDL_DEV_STATS                                      247
#define APPLE80211_IOC_LAST_ASSOC_HISTORY                                  248
#define APPLE80211_IOC_AWDL_COMMON_CHANNEL                                 249
#define APPLE80211_IOC_AWDL_PEERS_INFO                                     250
#define APPLE80211_IOC_TKO_PARAMS                                          251
#define APPLE80211_IOC_TKO_DUMP                                            252
#define APPLE80211_IOC_AWDL_NEARBY_LOG_TRIGGER                             253
#define APPLE80211_IOC_HW_SUPPORTED_CHANNELS                               254
#define APPLE80211_IOC_BTCOEX_PROFILE                                      255
#define APPLE80211_IOC_BTCOEX_PROFILE_ACTIVE                               256
#define APPLE80211_IOC_TRAP_INFO                                           257
#define APPLE80211_IOC_THERMAL_INDEX                                       258
#define APPLE80211_IOC_MAX_NSS_FOR_AP                                      259
#define APPLE80211_IOC_BTCOEX_2G_CHAIN_DISABLE                             260
#define APPLE80211_IOC_POWER_BUDGET                                        261
#define APPLE80211_IOC_AWDL_DFSP_CONFIG                                    262
#define APPLE80211_IOC_AWDL_DFSP_UCSA_CONFIG                               263
#define APPLE80211_IOC_SCAN_BACKOFF_REPORT                                 264
#define APPLE80211_IOC_OFFLOAD_TCPKA_ENABLE                                265
#define APPLE80211_IOC_RANGING_CAPS                                        266
#define APPLE80211_IOC_SUPPRESS_SCANS                                      267
#define APPLE80211_IOC_NAN_ENABLED                                         268
#define APPLE80211_IOC_NAN_INIT                                            269
#define APPLE80211_IOC_NAN_CLUSTER_ID                                      270
#define APPLE80211_IOC_NAN_DISC_BCN_PERIOD                                 271
#define APPLE80211_IOC_NAN_SYNC_BCN_PERIOD                                 272
#define APPLE80211_IOC_NAN_DW_AWAKE_PERIOD                                 273
#define APPLE80211_IOC_NAN_BCN_TX_CONTROL                                  274
#define APPLE80211_IOC_NAN_HOST_ELECTION                                   275
#define APPLE80211_IOC_NAN_RSSI_CLOSE                                      276
#define APPLE80211_IOC_NAN_RSSI_MIDDLE                                     277
#define APPLE80211_IOC_NAN_RSSI_5G_CLOSE                                   278
#define APPLE80211_IOC_NAN_RSSI_5G_MIDDLE                                  279
#define APPLE80211_IOC_NAN_DEVICE_INFO                                     280
#define APPLE80211_IOC_NAN_MASTER_PREFERENCE                               281
#define APPLE80211_IOC_NAN_RANDOM_FACTOR                                   282
#define APPLE80211_IOC_NAN_RANDOM_FACTOR_ROTATION                          283
#define APPLE80211_IOC_NAN_HOP_COUNT_TO_ANCHOR_MASTER                      284
#define APPLE80211_IOC_NAN_MASTER_RANK                                     285
#define APPLE80211_IOC_NAN_MAX_PEERS                                       286
#define APPLE80211_IOC_NAN_MASTER_CHANNEL                                  287
#define APPLE80211_IOC_NAN_SECONDARY_MASTER_CHANNEL                        288
#define APPLE80211_IOC_NAN_TX_RATE                                         289
#define APPLE80211_IOC_NAN_WARMUP_TIME                                     290
#define APPLE80211_IOC_NAN_CURRENT_ANCHOR_MASTER_RECORD                    291
#define APPLE80211_IOC_NAN_LAST_ANCHOR_MASTER_RECORD                       292
#define APPLE80211_IOC_COMPANION_SKYWALK_IF_CREATE                         293
#define APPLE80211_IOC_NAN_CW                                              294
#define APPLE80211_IOC_NAN_CW_RS                                           295
#define APPLE80211_IOC_NAN_SYNC_STATS                                      296
#define APPLE80211_IOC_NAN_HOP_COUNT_LIMIT                                 297
#define APPLE80211_IOC_NAN_AF_TX                                           298
#define APPLE80211_IOC_NAN_CHANNEL_SEQUENCE                                299
#define APPLE80211_IOC_NAN_PUBLISH                                         300
#define APPLE80211_IOC_NAN_CANCEL_PUBLISH                                  301
#define APPLE80211_IOC_NAN_PUBLISHED_SERVICE_LIST                          302
#define APPLE80211_IOC_NAN_SUBSCRIBE                                       303
#define APPLE80211_IOC_NAN_SUBSCRIBED_SERVICE_LIST                         304
#define APPLE80211_IOC_NAN_PUBLISHERS_LIST                                 305
#define APPLE80211_IOC_NAN_CANCEL_SUBSCRIBE                                306
#define APPLE80211_IOC_NAN_FOLLOW_UP_TRANSMIT                              307
#define APPLE80211_IOC_NAN_DISCOVERY_RESULT                                308
#define APPLE80211_IOC_NAN_FOLLOW_UP_RECEIVE                               309
#define APPLE80211_IOC_NAN_SERVICE_DISCOVERY_STATS                         310
#define APPLE80211_IOC_NAN_SERVICE_DESCRIPTOR_EXTENSION                    311
#define APPLE80211_IOC_NAN_RANGING_REQUEST                                 312
#define APPLE80211_IOC_NAN_RANGING_RESPONSE                                313
#define APPLE80211_IOC_NAN_RANGING_CANCEL                                  314
#define APPLE80211_IOC_NAN_DP_REQ                                          315
#define APPLE80211_IOC_NAN_DP_RESP                                         316
#define APPLE80211_IOC_NAN_SCHEDULE_UPDATE                                 317
#define APPLE80211_IOC_NAN_DP_END                                          318
#define APPLE80211_IOC_NAN_DP_CONF                                         319
#define APPLE80211_IOC_NAN_ADVERTISERS                                     320
#define APPLE80211_IOC_NAN_COMMITTED_AVAILABILITY                          321
#define APPLE80211_IOC_NAN_POTENTIAL_AVAILABILITY                          322
#define APPLE80211_IOC_NAN_DEVICE_CAPABILITY                               323
#define APPLE80211_IOC_NAN_NDC_AVAILABILITY                                324
#define APPLE80211_IOC_NAN_IMMUTABLE_AVAILABILITY                          325
#define APPLE80211_IOC_NAN_RANGING_AVAILABILITY                            326
#define APPLE80211_IOC_NAN_PEER_DATABASE                                   327
#define APPLE80211_IOC_NAN_CTL                                             328
#define APPLE80211_IOC_NAN_DP_INFO                                         329
#define APPLE80211_IOC_NAN_VENDOR_PAYLOAD                                  330
#define APPLE80211_IOC_NAN_ELECTION_ROLE                                   331
#define APPLE80211_IOC_NAN_QOS_PARAMS                                      332
#define APPLE80211_IOC_NAN_FAST_DISCOVERY                                  333
#define APPLE80211_IOC_RADIO_CHANSEQ_CONTROL                               334
#define APPLE80211_IOC_NAN_DATA_PATH_RESTORE                               335
#define APPLE80211_IOC_HOST_AP_MODE_HIDDEN                                 336
#define APPLE80211_IOC_LQM_CONFIG                                          337
#define APPLE80211_IOC_AWDL_CCA                                            338
#define APPLE80211_IOC_TRAP_CRASHTRACER_MINI_DUMP                          339
#define APPLE80211_IOC_TRAFFIC_REGISTRATION                                340
#define APPLE80211_IOC_AWDL_AF_SECONDARY_PAYLOAD_CAPABILITIES              341
#define APPLE80211_IOC_AWDL_OOB_SEC_PAYLOAD_REQUEST                        342
#define APPLE80211_IOC_AWDL_SIDECAR_STATISTICS                             343
#define APPLE80211_IOC_LLW_PARAMS                                          344
#define APPLE80211_IOC_HE_CAPABILITY                                       345
#define APPLE80211_IOC_BEACON_INFO                                         346
#define APPLE80211_IOC_SOFTAP_PARAMS                                       347
#define APPLE80211_IOC_CHIP_POWER_RANGE                                    348
#define APPLE80211_IOC_SOFTAP_TRIGGER_CSA                                  349
#define APPLE80211_IOC_SOFTAP_STATS                                        350
#define APPLE80211_IOC_AWDL_SIDECAR_DIAGNOSTICS                            351
#define APPLE80211_IOC_SOFTAP_WIFI_NETWORK_INFO_IE                         352
#define APPLE80211_IOC_NSS                                                 353
#define APPLE80211_IOC_COMPANION_SKYWALK_LINK_STATE                        354
#define APPLE80211_IOC_NAN_LLW_PARAMS                                      355
#define APPLE80211_IOC_HP2P_CAPS                                           356
#define APPLE80211_IOC_RLLW_STATS                                          357
#define APPLE80211_IOC_BTCOEX_DISABLE_ULOFDMA                              358
#define APPLE80211_IOC_HW_ADDR                                             359
#define APPLE80211_IOC_SCAN_CONTROL                                        360
#define APPLE80211_IOC_HE_MCS_INDEX_SET                                    361
#define APPLE80211_IOC_CHIP_DIAGS                                          362
#define APPLE80211_IOC_USB_HOST_NOTIFICATION                               363
#define APPLE80211_IOC_LOWLATENCY_STATISTICS                               364
#define APPLE80211_IOC_DISPLAY_STATE                                       365
#define APPLE80211_IOC_NAN_OOB_AF_TX                                       366
#define APPLE80211_IOC_NAN_DATA_PATH_KEEP_ALIVE_IDENTIFIER                 367
#define APPLE80211_IOC_SET_MAC_ADDRESS                                     368
#define APPLE80211_IOC_ASSOCIATE_EXTENDED_RESULT                           369
#define APPLE80211_IOC_AWDL_AIRPLAY_STATISTICS                             370
#define APPLE80211_IOC_HP2P_CTRL                                           371
#define APPLE80211_IOC_REQUEST_BSS_BLACKLIST                               372
#define APPLE80211_IOC_ASSOC_READY_STATUS                                  373
#define APPLE80211_IOC_TXRX_CHAIN_INFO                                     374
#define APPLE80211_IOC_MIMO_STATUS                                         375
#define APPLE80211_IOC_CUR_PMK                                             376
#define APPLE80211_IOC_DYNSAR_DETAIL                                       377
#define APPLE80211_IOC_RANDOMISATION_STATUS                                378
#define APPLE80211_IOC_COUNTRY_CHANNELS_INFO                               379

#define APPLE80211_IOC_CARD_SPECIFIC                                       UINT32_MAX

// Bump this value when structures change
#define APPLE80211_VERSION 1

struct apple80211_ssid_data
{
    u_int32_t version;
    u_int32_t ssid_len;
    u_int8_t  ssid_bytes[APPLE80211_MAX_SSID_LEN];
};

struct apple80211_virt_if_create_data
{
    uint32_t version;
    uint8_t  mac[APPLE80211_ADDR_LEN];
    uint32_t role;
    uint8_t  bsd_name[IFNAMSIZ];
};

struct apple80211_virt_if_delete_data
{
    uint32_t version;
    uint8_t  bsd_name[IFNAMSIZ];
};

#define IEEE80211_HT_MCS_MASK_LEN 10

/*! @struct ieee80211_mcs_info
 *   @abstract MCS Information structure.
 *   @field rx_mask The RX mask.
 *   @field rx_highest Highest supported RX rate.  If set, it represents the highest supported RX data rate in units of 1 Mbps. If this field is 0 this value should not be used to consider the highest RX data rate supported.
 *   @field tx_params: The TX parameters.
 */

struct ieee80211_mcs_info
{
    uint8_t  rx_mask[IEEE80211_HT_MCS_MASK_LEN];
    uint16_t rx_highest;
    uint8_t  tx_params;
    uint8_t  reserved[3];
} __attribute__((packed));

/*! @struct ieee80211_ht_cap
 *   @abstract A struct describing HT capabilities.
 *   @discussion This structure is the "HT capabilities element" as described in 802.11n D5.0 7.3.2.57.
*/

struct ieee80211_ht_capability_ie
{
    uint8_t  element_id;
    uint8_t  length;
    uint16_t cap_info;
    uint8_t  ampdu_params_info;

    /* 16 bytes MCS information */
    ieee80211_mcs_info mcs;

    uint16_t extended_ht_cap_info;
    uint32_t tx_BF_cap_info;
    uint8_t  antenna_selection_info;
} __attribute__((packed));

struct apple80211_ht_capability
{
    uint32_t    version;
    uint8_t     hc_id;              /* element ID */
    uint8_t     hc_len;             /* length in bytes */
    uint16_t    hc_cap;             /* HT caps (see below) */
    uint8_t     hc_param;           /* HT params (see below) */
    uint8_t     hc_mcsset[16];      /* supported MCS set */
    uint16_t    hc_extcap;          /* extended HT capabilities */
    uint32_t    hc_txbf;            /* txbf capabilities */
    uint8_t     hc_antenna;         /* antenna capabilities */
} __attribute__((packed));

/*! @struct ieee80211_vht_mcs_info
 *   @abstract VHT MCS information.
 *   @field rx_mcs_map: RX MCS map 2 bits for each stream, total 8 streams
 *   @field rx_highest: Indicates highest long GI VHT PPDU data rate STA can receive. Rate expressed in units of 1 Mbps. If this field is 0 this value should not be used to consider the highest RX data rate supported. The top 3 bits of this field indicate the Maximum NSTS,total (a beamformee capability.)
 *   @field tx_mcs_map: TX MCS map 2 bits for each stream, total 8 streams
 *   @field tx_highest: Indicates highest long GI VHT PPDU data rate STA can transmit. Rate expressed in units of 1 Mbps. If this field is 0 this value should not be used to consider the highest TX data rate supported. The top 2 bits of this field are reserved, the 3rd bit from the top indiciates VHT Extended NSS BW Capability.
 */
struct ieee80211_vht_mcs_info
{
    uint16_t rx_mcs_map;
    uint16_t rx_highest;
    uint16_t tx_mcs_map;
    uint16_t tx_highest;
};

struct apple80211_vht_capability
{
    uint32_t    version;
    uint8_t     element_id;
    uint8_t     length;
    uint32_t    cap_info;
    uint8_t     vhc_mcs_set[8];
};

struct apple80211_channel_data
{
    u_int32_t                    version;
    struct apple80211_channel    channel;
};

struct apple80211_bssid_data
{
    u_int32_t            version;
    struct ether_addr    bssid;
};

struct apple80211_capability_data
{
    u_int32_t    version;
    u_int8_t     capabilities[11];
};

struct apple80211_state_data
{
    u_int32_t    version;
    u_int32_t    state;
};

struct apple80211_rssi_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    rssi_unit;
    int32_t      rssi[APPLE80211_MAX_RADIO];        // control channel
    int32_t      aggregate_rssi;                    // aggregate control channel rssi
    int32_t      rssi_ext[APPLE80211_MAX_RADIO];    // extension channel rssi
    int32_t      aggregate_rssi_ext;                // aggregate extension channel rssi
};

struct apple80211_power_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    power_state[APPLE80211_MAX_RADIO];
};

struct apple80211_assoc_result_data
{
    u_int32_t    version;
    u_int32_t    result;
};

struct apple80211_assoc_status_data
{
    u_int32_t    version;
    u_int32_t    status;
};

struct apple80211_rate_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    rate[APPLE80211_MAX_RADIO];
};

struct apple80211_status_dev_data
{
    u_int32_t    version;
    u_int8_t     dev_name[MAXPATHLEN];
};

struct apple80211_powersave_data
{
    u_int32_t    version;
    u_int32_t    powersave_level;
};

struct apple80211_protmode_data
{
    u_int32_t    version;
    u_int32_t    protmode;
    u_int32_t    threshold;        // bytes
};

struct apple80211_txpower_data
{
    u_int32_t    version;
    u_int32_t    txpower_unit;
    int32_t      txpower;
};

struct apple80211_phymode_data
{
    u_int32_t    version;
    u_int32_t    phy_mode;            // vector of supported phy modes
    u_int32_t    active_phy_mode;     // current active phy mode
};

struct apple80211_opmode_data
{
    u_int32_t    version;
    u_int32_t    op_mode;
};

struct apple80211_noise_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    u_int32_t    noise_unit;
    int32_t      noise[APPLE80211_MAX_RADIO];        // control channel
    int32_t      aggregate_noise;                    // aggregate control channel noise
    int32_t      noise_ext[APPLE80211_MAX_RADIO];    // extension channel noise
    int32_t      aggregate_noise_ext;                // aggregate extension channel noise
};

struct apple80211_intmit_data
{
    u_int32_t    version;
    u_int32_t    int_mit;
};

struct apple80211_authtype_data
{
    u_int32_t    version;
    u_int32_t    authtype_lower;    //    apple80211_authtype_lower
    u_int32_t    authtype_upper;    //    apple80211_authtype_upper
};

struct apple80211_sup_channel_data
{
    u_int32_t                    version;
    u_int32_t                    num_channels;
    struct apple80211_channel    supported_channels[APPLE80211_MAX_CHANNELS];
};

struct apple80211_roam_threshold_data
{
    u_int32_t threshold;
    u_int32_t count;
};

struct apple80211_locale_data
{
    u_int32_t    version;
    u_int32_t    locale;
};

struct apple80211_scan_data
{
    u_int32_t                    version;
    u_int32_t                    bss_type;                            // apple80211_apmode 4
    struct ether_addr            bssid;                               // target BSSID 8
    u_int32_t                    ssid_len;                            // length of the SSID 14
    u_int8_t                     ssid[APPLE80211_MAX_SSID_LEN];       // direct scan ssid or AirDrop scan ssid like "Air-xxxx" 18
    u_int32_t                    scan_type;                           // apple80211_scan_type 50
    u_int32_t                    phy_mode;                            // apple80211_phymode vector 54
    u_int16_t                    dwell_time;                          // time to spend on each channel (ms) 58
    u_int32_t                    rest_time;                           // time between scanning each channel (ms) 60
    u_int32_t                    num_channels;                        // 0 if not passing in channels 64
    struct apple80211_channel    channels[APPLE80211_MAX_CHANNELS];   // channel list 68
    // 1608
};
//static_assert(sizeof(apple80211_scan_data) == 2184 );

struct apple80211_scan_multiple_data
{
    uint32_t                  version;
    uint32_t                  ap_mode; // apple80211_apmode, 4
    uint32_t                  ssid_count; // 8
    apple80211_ssid_data      ssids[10]; // 12
    uint32_t                  bssid_count; // 412
    ether_addr                bssids[16]; // 416
    uint32_t                  scan_type; // 512
    uint32_t                  phy_mode; // 516
    uint32_t                  dwell_time; //520
    uint32_t                  rest_time; // 524
    uint32_t                  num_channels; //528
    struct apple80211_channel channels[APPLE80211_MAX_CHANNELS * 2]; //532
    
};
//static_assert(sizeof(apple80211_scan_multiple_data) == 2080 );

struct apple80211_link_changed_event_data
{
    bool       isLinkDown; // 0
    uint32_t   rssi;       // 4
    uint16_t   snr;        // 8
    uint16_t   nf;         // 10
    char       cca;        // 12
    bool       voluntary;  // 16
    uint32_t   reason;     // 20
};

struct apple80211_apmode_data
{
    u_int32_t    version;
    u_int32_t    apmode;
};

struct apple80211_assoc_data
{
    u_int32_t                version;
    u_int16_t                ad_mode;          // apple80211_apmode
    u_int16_t                ad_auth_lower;    // apple80211_authtype_lower
    u_int16_t                ad_auth_upper;    // apple80211_authtype_upper
    u_int32_t                ad_ssid_len;
    u_int8_t                 ad_ssid[ APPLE80211_MAX_SSID_LEN ];
    struct ether_addr        ad_bssid;         // prefer over ssid if not zeroed
    struct apple80211_key    ad_key;
    uint16_t                 ad_rsn_ie_len;
    u_int8_t                 ad_rsn_ie[ APPLE80211_MAX_RSN_IE_LEN + 1 ];
    u_int32_t                ad_flags;         // apple80211_assoc_flags
};

struct apple80211_deauth_data
{
    u_int32_t            version;
    u_int32_t            deauth_reason;    // reason code
    struct ether_addr    deauth_ea;        // BSSID of AP
};

struct apple80211_countermeasures_data
{
    u_int32_t    version;
    u_int32_t    enabled;
};

struct apple80211_frag_threshold_data
{
    u_int32_t    version;
    u_int32_t    threshold;    // bytes
};

struct apple80211_rate_set_data
{
    u_int32_t                version;
    u_int16_t                num_rates;
    struct apple80211_rate   rates[APPLE80211_MAX_RATES];
};

struct apple80211_short_slot_data
{
    u_int32_t    version;
    u_int8_t     mode;
};

struct apple80211_retry_limit_data
{
    u_int32_t    version;
    u_int32_t    limit;
};

struct apple80211_antenna_data
{
    u_int32_t    version;
    u_int32_t    num_radios;
    int32_t      antenna_index[APPLE80211_MAX_RADIO];
};

struct apple80211_dtim_int_data
{
    u_int32_t    version;
    u_int32_t    interval;
};

struct apple80211_sta_data
{
    u_int32_t                    version;
    u_int32_t                    num_stations;
    struct apple80211_station    station_list[APPLE80211_MAX_STATIONS];
};

struct apple80211_version_data
{
    u_int32_t    version;
    u_int16_t    string_len;
    char         string[APPLE80211_MAX_VERSION_LEN];
};

struct apple80211_rom_data
{
    u_int32_t    version;
    u_int32_t    rom_len;
    u_int8_t     rom[1];    // variable length
};

struct apple80211_rand_data
{
    u_int32_t    version;
    u_int32_t    rand;
};

struct apple80211_rsn_ie_data
{
    u_int32_t    version;
    u_int16_t    len;
    u_int8_t     ie[ APPLE80211_MAX_RSN_IE_LEN ];
};

struct apple80211_ap_ie_data
{
    u_int32_t    version;
    u_int32_t    len;
    u_int8_t     *ie_data;
};

struct apple80211_stats_data
{
    u_int32_t    version;
    u_int32_t    tx_frame_count;
    u_int32_t    tx_errors;
    u_int32_t    rx_frame_count;
    u_int32_t    rx_errors;
};

struct apple80211_country_code_data
{
    u_int32_t    version;
    u_int8_t     cc[APPLE80211_MAX_CC_LEN];
};

struct apple80211_last_rx_pkt_data
{
    u_int32_t         version;
    u_int32_t         rate;
    int32_t           rssi;
    u_int32_t         num_streams;    // number of spatial streams
    struct ether_addr sa;             // source address
};

struct apple80211_radio_info_data
{
    u_int32_t    version;
    u_int32_t    count;        // number of rf chains
};

struct apple80211_guard_interval_data
{
    u_int32_t    version;
    u_int32_t    interval;    // apple80211_guard_interval
};

struct apple80211_mcs_data
{
    u_int32_t    version;
    u_int32_t    index;        // 0 to APPLE80211_MAX_MCS_INDEX
};

struct apple80211_rifs_data
{
    u_int32_t    version;
    u_int32_t    enabled;
};

struct apple80211_ldpc_data
{
    u_int32_t    version;
    u_int32_t    enabled;
};

struct apple80211_msdu_data
{
    u_int32_t    version;
    u_int32_t    max_length;        // 3839 or 7935 bytes
};

struct apple80211_mpdu_data
{
    u_int32_t    version;
    u_int32_t    max_factor;        // 0 - APPLE80211_MAX_MPDU_FACTOR
    u_int32_t    max_density;       // 0 - APPLE80211_MAX_MPDU_DENSITY
};

struct apple80211_block_ack_data
{
    u_int32_t   version;
    u_int8_t    ba_enabled;             // block ack enabled
    u_int8_t    immediate_ba_enabled;   // immediate block ack enabled
    u_int8_t    cbba_enabled;           // compressed bitmap block ack enabled
    u_int8_t    implicit_ba_enabled;    // implicit block ack enabled
};

struct apple80211_pls_data
{
    u_int32_t    version;
    u_int32_t    enabled;    // phy level spoofing enabled
};

struct apple80211_psmp_data
{
    u_int32_t    version;
    u_int32_t    enabled;
};

struct apple80211_physubmode_data
{
    u_int32_t    version;
    u_int32_t    phy_mode;       // one apple80211_phymode
    u_int32_t    phy_submode;    // one apple80211_physubmode
    u_int32_t    flags;          // apple80211_channel_flag vector
};

struct apple80211_mcs_index_set_data
{
    u_int32_t   version;
    u_int8_t    mcs_set_map[APPLE80211_MAP_SIZE( APPLE80211_MAX_MCS_INDEX + 1 )];
};

struct apple80211_vht_mcs_index_set_data
{
    u_int32_t   version;
    u_int16_t   mcs_map;
} __attribute__((packed));

struct apple80211_mcs_vht_data
{
    u_int32_t   version;
    u_int32_t   index;
    u_int32_t   nss;
    u_int32_t   bw;
    u_int32_t   guard_interval;
} __attribute__((packed));

struct apple80211_wow_parameter_data
{
    u_int32_t                     version;
    u_int8_t                      wake_cond_map[APPLE80211_MAP_SIZE( APPLE80211_MAX_WAKE_COND + 1 )];
    u_int32_t                     beacon_loss_time;
    u_int32_t                     pattern_count;
    struct apple80211_wow_pattern patterns[APPLE80211_MAX_WOW_PATTERNS];
};

struct apple80211_40mhz_intolerant_data
{
    u_int32_t    version;
    u_int32_t    enabled;    // bit enabled or not
};

struct apple80211_tx_nss_data
{
    uint32_t    version;
    uint8_t     nss;
};

struct apple80211_nss_data
{
    uint32_t    version;
    uint8_t     nss;
};

struct apple80211_awdl_peer_traffic_registration
{
    uint32_t    version;
    void        *addr;
    uint32_t    name_len;
    char        name[152];
    uint32_t    active;
} __attribute__((packed));

struct apple80211_awdl_election_metric
{
    uint32_t    version;
    uint32_t    metric;
} __attribute__((packed));

struct apple80211_awdl_sync_enabled
{
    uint32_t    version;
    uint32_t    unk1;
    uint32_t    enabled;
} __attribute__((packed));

struct apple80211_awdl_sync_frame_template
{
    uint32_t    version;
    uint32_t    payload_len;
    void        *payload;
} __attribute__((packed));

struct apple80211_awdl_bssid
{
    uint32_t    version;
    uint8_t     bssid[APPLE80211_ADDR_LEN];
    uint8_t     unk_mac[APPLE80211_ADDR_LEN];
} __attribute__((packed));

struct apple80211_awdl_channel
{
    uint16_t    chan_spec;
    uint8_t     chan_num;
    uint8_t     indoor_restric;
    uint8_t     radar_dfs;
    uint8_t     passive;
    uint8_t     support_40Mhz;
    uint8_t     support_80Mhz;
    uint8_t     z;
    uint32_t    per_chan;
    uint32_t    chan_bitmap;
} __attribute__((packed));

struct apple80211_channels_info
{
    uint32_t    version;
    uint32_t    unk1;
    uint16_t    num_chan_specs;
    uint16_t    chan_spec[APPLE80211_MAX_CHANNELS];
    uint8_t     chan_num[APPLE80211_MAX_CHANNELS];
    uint8_t     indoor_restric[APPLE80211_MAX_CHANNELS];
    uint8_t     radar_dfs[APPLE80211_MAX_CHANNELS];
    uint8_t     passive[APPLE80211_MAX_CHANNELS];
    uint8_t     support_40Mhz[APPLE80211_MAX_CHANNELS];
    uint8_t     support_80Mhz[APPLE80211_MAX_CHANNELS];
    uint8_t     z[APPLE80211_MAX_CHANNELS];
    uint8_t     pad[386];
    uint32_t    per_chan[APPLE80211_MAX_CHANNELS];
    uint32_t    chan_bitmap[APPLE80211_MAX_CHANNELS];
} __attribute__((packed));

//static_assert(__offsetof(struct apple80211_channels_info, chan_num) == 0x10A, "invalid offset");   //wf_chspec_ctlchan
//static_assert(__offsetof(struct apple80211_channels_info, indoor_restric) == 0x18A, "invalid offset"); //wlc_restricted_chanspec
//static_assert(__offsetof(struct apple80211_channels_info, radar_dfs) == 0x20A, "invalid offset");  //wlc_radar_chanspec
//static_assert(__offsetof(struct apple80211_channels_info, passive) == 0x28A, "invalid offset");    //wlc_quiet_chanspec
//static_assert(__offsetof(struct apple80211_channels_info, support_40Mhz) == 0x30A, "invalid offset");
//static_assert(__offsetof(struct apple80211_channels_info, support_80Mhz) == 0x38A, "invalid offset");
//static_assert(__offsetof(struct apple80211_channels_info, per_chan) == 0x60C, "invalid offset");

struct apple80211_peer_cache_maximum_size
{
    uint32_t    version;
    uint32_t    max_peers;
} __attribute__((packed));

struct apple80211_awdl_election_id
{
    uint32_t    version;
    uint32_t    election_id;
} __attribute__((packed));

struct apple80211_awdl_master_channel
{
    uint32_t    version;
    uint32_t    master_channel;
} __attribute__((packed));

struct apple80211_awdl_secondary_master_channel
{
    uint32_t    version;
    uint32_t    secondary_master_channel;
} __attribute__((packed));

struct apple80211_awdl_min_rate
{
    uint32_t    version;
    uint8_t    min_rate;
} __attribute__((packed));

struct apple80211_awdl_election_rssi_thresholds
{
    uint32_t    version;
    uint32_t    unk1;
    uint32_t    unk2;
    uint32_t    unk3;
} __attribute__((packed));

struct apple80211_awdl_sync_channel_sequence
{
    uint32_t           version;
    uint8_t            pad1;
    uint8_t            length;             // 5
    uint8_t            encoding;           // 6
    uint8_t            step_count;         // 7
    uint8_t            duplicate_count;    // 8
    uint8_t            fill_channel;       // 9
    uint8_t            pad2[6];
    apple80211_channel channels[APPLE80211_MAX_AWDL_CHANNELS];
} __attribute__((packed));

static_assert(sizeof(apple80211_awdl_sync_channel_sequence) == 400);

struct apple80211_awdl_presence_mode
{
    uint32_t    version;
    uint32_t    mode;
} __attribute__((packed));

struct apple80211_awdl_extension_state_machine_parameter
{
    uint32_t    version;
    uint32_t    unk1;
    uint32_t    unk2;
    uint32_t    unk3;
    uint32_t    unk4;
} __attribute__((packed));

struct apple80211_awdl_sync_state
{
    uint32_t    version;
    uint32_t    state;
} __attribute__((packed));

struct apple80211_awdl_sync_params
{
    uint32_t    version;
    uint32_t    availability_window_length;
    uint32_t    availability_window_period;
    uint32_t    extension_length;
    uint32_t    synchronization_frame_period;
} __attribute__((packed));

struct apple80211_awdl_cap
{
    uint32_t    version;
    uint8_t     cap;
} __attribute__((packed));

struct apple80211_awdl_af_tx_mode
{
    uint32_t    version;
    uint64_t    mode;
} __attribute__((packed));

#define AWDL_OOB_AF_PARAMS_SIZE 38

struct apple80211_awdl_oob_request
{
    uint32_t    version;
    uint32_t    unk1;               // 4
    uint32_t    unk2;               // 8
    uint32_t    unk3;               // 12
    uint32_t    unk4;               // 16
    uint16_t    pad1;
    uint32_t    unk5;               // 22
    uint16_t    unk6;               // 26
    uint32_t    pad2;
    uint32_t    unk7;               // 32
    uint32_t    pad3;
    uint16_t    data_len;           // 40
    uint32_t    pad4;
    uint16_t    unk9;               // 44
    uint8_t     data[1782];         // 48
} __attribute__((packed));

struct apple80211_roam_profile
{
    int8_t      flags;
    int8_t      trigger;
    int8_t      rssi_lower;
    int8_t      rssi_boost_delta;
    int8_t      rssi_boost_thresh;
    int8_t      delta;
    uint16_t    backoff_multiplier;
    uint16_t    full_scan_period;
    uint16_t    init_scan_period;
    uint16_t    nfscan;
    uint16_t    max_scan_period;
} __attribute__((packed));

struct apple80211_roam_profile_band_data
{
    uint32_t    version;
    uint32_t    flags;          // 4 (0x2, 0x4)
    uint32_t    profile_cnt;    // 8
    struct apple80211_roam_profile profiles[4];
} __attribute__((packed));

static_assert(sizeof(struct apple80211_roam_profile_band_data) == 76, "roam data size error");

struct apple80211_ie_data
{
    uint32_t    version;
    uint32_t    frame_type_flags;   // 4
    uint32_t    add;                // 8
    uint32_t    signature_len;      // 12
    uint32_t    ie_len;             // 16
    uint32_t    pad1;               // 20
    uint8_t     ie[2048];
} __attribute__((packed));

struct apple80211_p2p_listen_data
{
    uint32_t    version;
    uint32_t    pad1;
    uint32_t    channel;        // 8
    uint32_t    flags;          // 12
    uint32_t    duration;       // 16
} __attribute__((packed));

struct apple80211_p2p_go_conf_data
{
    uint32_t    version;
    uint32_t    auth_upper;     // 4 should equal to 1
    uint32_t    auth_lower;     // 6 should non zero
    void        *dynbcn;        // 8
    uint32_t    channel;        // 12
    uint32_t    bcn_len;        // 16
    uint32_t    ssid_len;       // 20
    uint8_t     ssid[32];       // 24
    uint32_t    suppress_beacon;// 56 security:1,4
} __attribute__((packed));

struct apple80211_sta_roam_data
{
    uint32_t    version;
    uint8_t     rcc_channels;
    uint8_t     unk1;
    uint8_t     taget_channel;
    uint8_t     target_bssid[APPLE80211_ADDR_LEN];
} __attribute__((packed));

#endif // _APPLE80211_IOCTL_H_
