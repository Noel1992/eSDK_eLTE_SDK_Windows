/*
  The oRTP library is an RTP (Realtime Transport Protocol - rfc3550) stack.
  Copyright (C) 2011-2012 Aymeric MOIZARD amoizard_at_osip.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */

#pragma once

#include <ortp/rtpsession.h>

#ifdef __cplusplus
extern "C"{
#endif

  struct StunTransaction {
    UInt96 tid;
    char request[STUN_MAX_MESSAGE_SIZE];
    int requestlen;
    int state;
    int nbretransmission;
    uint64_t timeout;
  };

typedef enum {
    TurnSessionInit = 0,
    TurnSessionConnected,
    TurnSessionSentAllocate,
    TurnSessionSentChanBind,
    TurnSessionStable,
    TurnSessionSentRefresh,
    TurnSessionSentBindRefresh,
    TurnSessionSentDeleteAlloc,
    TurnSessionTerminated,
    TurnSessionFailed
} TurnSessionState;

  struct TurnSocketData {
    int stun_indication_received;
    int fd;
    int proto;
    int srcport;
    int use_relay;
    StunAddress4 peerAddress;

    TurnSessionState state;
    queue_t send_q;
    int send_remain;
    mblk_t *recv_m;
    int recv_size;
    int recv_need;
    int recv_left;
    int skipping;
    int have_turn_msg;

    /* TURN auth data */
    char auth_username[514]; // MAX 513 bytes
    char auth_password[514];
    char auth_realm[763]; //128 bytes in UTF8
    char auth_nonce[763]; //128 bytes in UTF8

    int bad_nonce_count;

    struct StunTransaction refresh;
    struct StunTransaction createpermission;
    uint16_t channelbind_number;
    int channelbind_state;
    uint16_t max_channelbind_number;

    StunAddress4 relay_addr;
    StunAddress4 mapped_addr;
    StunAddress4 bind_addr;

    time_t alloc_expires;
    time_t channel_expires;
};

  /* list of state for STUN connectivity check */
#define ICE_PRUNED -1
#define ICE_FROZEN 0
#define ICE_WAITING 1
#define ICE_IN_PROGRESS 2 /* STUN request was sent */
#define ICE_SUCCEEDED 3
#define ICE_FAILED 4 /* no answer or unrecoverable failure */


  struct SdpCandidate {
    /* mandatory attributes: draft 19 */
    char foundation[33];
    int component_id;
    char transport[20];
    int priority;
    char conn_addr[64];
    int conn_port;
    char cand_type[20];
    char rel_addr[64];
    int rel_port;

    /* optionnal attributes: draft 19 */
    char extension_attr[512]; /* *(SP extension-att-name SP extension-att-value) */
  };

  struct CandidatePair {

    struct SdpCandidate local_candidate;
    struct SdpCandidate remote_candidate;
    long long pair_priority;
    /* additionnal information */
    int rem_controlling;

    struct StunTransaction connectivitycheck;
    struct StunTransaction createpermission;

    int nominated_pair;
    unsigned int channelbind_number;
    int channelbind_state;
    time_t channel_expires;
  };

#define MAX_ICE_CANDIDATES 10

  struct IceCheckList {
    struct _OList *cand_pairs;
    int nominated_pair_index;

    char loc_ice_ufrag[256];
    char loc_ice_pwd[256];
    char rem_ice_ufrag[256];
    char rem_ice_pwd[256];

    int rem_controlling;
    uint64_t tiebreak_value;

#define ICE_CL_RUNNING 0
#define ICE_CL_COMPLETED 1
#define ICE_CL_FAILED 2
    int state;

    int RTO;
    int Ta;
    uint64_t keepalive_time;
    char remote_conn_ip[64];
    int remote_conn_port;
    char local_conn_ip[64];
    int local_conn_port;
  };

struct _RtpTurnData {
    struct TurnSocketData rtp;
    struct TurnSocketData rtcp;
    uint64_t rtp_cur_time;
    uint64_t rtcp_cur_time;
};

typedef struct _RtpTurnData RtpTurnData;

RtpTurnData *rtp_turn_data_new(const char *relay_server, char *username, char *password, int srcport);
void rtp_turn_data_destroy(RtpTurnData *data);

int rtp_turn_get_relay_addr(RtpTurnData *data, StunAddress4 *relay_addr);

int turnBuildAllocate(char* buf, int buf_len, const StunAtrString *username, const StunAtrString *password, const StunAtrString *realm, const StunAtrString *nonce, const TurnAtrReservationToken* resToken, bool_t reservePortPair);

int turnBuildChannelBindOrPermission(int use_channelbind, char* buf, int buf_len, const StunAtrString *username, const StunAtrString *password, unsigned int chanNumber, const StunAddress4 *peerAddress, const StunAtrString *realm, const StunAtrString *nonce, UInt96 *tid);

int turnBuildRefresh(char* buf, int buf_size, const StunAtrString *username, const StunAtrString *password, const StunAtrString *realm, const StunAtrString *nonce, int lifetime, UInt96 *tid);

#ifdef __cplusplus
}
#endif
