 /*
  The oRTP library is an RTP (Realtime Transport Protocol - rfc3550) stack.
  Copyright (C) 2001  Simon MORLAT simon.morlat@linphone.org

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

/** 
 * \file rtpsession.h
 * \brief The RtpSession api
 *
 * The RtpSession objects represent a RTP session: once it is configured with 
 * local and remote network addresses and a payload type is given, it let you send
 * and recv a media stream.
**/


#ifndef RTPSESSION_H
#define RTPSESSION_H


#include <ortp/port.h>
#include <ortp/rtp.h>
#include <ortp/payloadtype.h>
#include <ortp/rtcp.h>
#include <ortp/str_utils.h>
#include <ortp/rtpsignaltable.h>
#include <ortp/event.h>
#include <ortp/rtptransport.h>
#include <ortp/stun.h>
#include <ortp/turn.h>

#define MAX_FB_NUM 200

typedef enum {
	RTP_SESSION_RECVONLY,
	RTP_SESSION_SENDONLY,
	RTP_SESSION_SENDRECV
} RtpSessionMode;


/*! Jitter buffer parameters
*/
typedef struct _JBParameters{
	int min_size; /**< in milliseconds*/
	int nom_size; /**< idem */
	int max_size; /**< idem */
	bool_t adaptive;
	bool_t pad[3];
	int max_packets; /**< max number of packets allowed to be queued in the jitter buffer */
} JBParameters;

typedef struct _JitterControl
{
	int count;
	int jitt_comp;   /* the user jitt_comp in miliseconds*/
	int jitt_comp_ts; /* the jitt_comp converted in rtp time (same unit as timestamp) */
	int adapt_jitt_comp_ts;
	int64_t slide;
	int64_t prev_slide;
	float jitter;
	float max_jitter;
	int cum_late_packet;
	float cur_max_late_packet;
	int64_t olddiff;
	float inter_jitter;	/* interarrival jitter as defined in the RFC */
	int corrective_step;
	int corrective_slide;
	bool_t adaptive;
	bool_t enabled;
	int burst_factor;
	int clock_rate;
	uint64_t average_buffer_size;
	int average_buffer_size_count;

	/* lijinglai add for rtprtx b */
	int clear_sumjitter;
	int sumjitter_param;
	int64_t sumjitter;
	int64_t ealsumjitter_r;
	int64_t vesumjitter_r;
	/* lijinglai add for rtprtx e */

    int64_t m_llRtpTimeStampNum;               // RTP timestamp翻转次数
    int64_t m_llUserTimeStampNum;              // User timestamp翻转次数
    int64_t m_llRtpTimeStampOffset;            // ssrc变化导致timestamp就地翻转偏移量
} JitterControl;

typedef struct _RtpStream
{
	ortp_socket_t socket;
	struct _RtpTransport *tr; 
	int sockfamily;
	int max_rq_size;
	int time_jump;
	uint32_t ts_jump;
	queue_t rq;
	queue_t tev_rq;
	mblk_t *cached_mp;
	int loc_port;
#ifdef ORTP_INET6
	struct sockaddr_storage rem_addr;
#else
	struct sockaddr_in rem_addr;
#endif
	int rem_addrlen;
	void *QoSHandle;
	unsigned long QoSFlowID;
	JitterControl jittctl;
	uint32_t snd_time_offset;/*the scheduler time when the application send its first timestamp*/	
	uint32_t snd_ts_offset;	/* the first application timestamp sent by the application */
	uint32_t snd_rand_offset;	/* a random number added to the user offset to make the stream timestamp*/
	uint32_t snd_last_ts;	/* the last stream timestamp sended */
	uint32_t rcv_time_offset; /*the scheduler time when the application ask for its first timestamp*/
	uint32_t rcv_ts_offset;  /* the first stream timestamp */
	uint32_t rcv_query_ts_offset;	/* the first user timestamp asked by the application */
	uint32_t rcv_last_ts;	/* the last stream timestamp got by the application */
    uint32_t m_uiRecvLastTimeStampInc;	/* the last stream timestamp got by the application, not reversed*/
	uint32_t rcv_last_app_ts; /* the last application timestamp asked by the application */	
	uint32_t rcv_last_ret_ts; /* the timestamp of the last sample returned (only for continuous audio)*/
	uint32_t hwrcv_extseq; /* last received on socket extended sequence number */
	uint32_t hwrcv_seq_at_last_SR;
	uint32_t hwrcv_since_last_SR;
	uint32_t last_rcv_SR_ts;     /* NTP timestamp (middle 32 bits) of last received SR */
	uint32_t last_rcv_SR_rtp;    /*added by tanghailang*/
	struct timeval last_rcv_SR_time;   /* time at which last SR was received  */
	uint16_t snd_seq; /* send sequence number */
	uint32_t last_rtcp_report_snt_r;	/* the time of the last rtcp report sent, in recv timestamp unit */
	uint32_t last_rtcp_report_snt_s;	/* the time of the last rtcp report sent, in send timestamp unit */
	uint32_t rtcp_report_snt_interval; /* the interval in timestamp unit between rtcp report sent */
	uint32_t last_rtcp_packet_count; /*the sender's octet count in the last sent RTCP SR*/
	uint32_t sent_payload_bytes; /*used for RTCP sender reports*/
	unsigned int sent_bytes; /* used for bandwidth estimation */
	struct timeval send_bw_start; /* used for bandwidth estimation */
	unsigned int recv_bytes; /* used for bandwidth estimation */
	struct timeval recv_bw_start; /* used for bandwidth estimation */
	rtp_stats_t stats;
    rtcp_stats_t rtcp_stat;  /*added by tanghailang*/	
    rtp_extended_stats_t extended_stats;
	int recv_errno;
	int send_errno;
	int snd_socket_size;
	int rcv_socket_size;
	int ssrc_changed_thres;
	int loss_rate;   /* added by tanghailang for lossrate calculation */
	int frame_rate;  /* added by tanghailang for framerate calculation */
	
	/* lijinglai add for rtprtx b */
	uint32_t hwrcv_seq_at_last_RR;
	uint32_t hwrcv_since_last_RR;
	uint64_t last_compute_lossrate_notrans_time;
	float    lossrate_noretran;
	/* lijinglai add for rtprtx e */
}RtpStream;

typedef struct _RtcpStream
{
	ortp_socket_t socket;
	int sockfamily;
	mblk_t *cached_mp;
#ifdef ORTP_INET6
	struct sockaddr_storage rem_addr;
#else
	struct sockaddr_in rem_addr;
#endif
	int rem_addrlen;
	bool_t enabled; /*tells whether we can send RTCP packets */
} RtcpStream;

typedef struct _RtpSession RtpSession;

struct _tc_qos_ifc;

struct _tc_qos {
	struct _tc_qos_ifc *ifc;
	int num_ifc;
};

/* lijinglai add for rtprtx b */
typedef struct _RtpRtxItem
{
	uint16_t seq;
	uint32_t fb_count;
    uint32_t fb_max;
    uint64_t recv_time;
    uint64_t first_fb_time;
    uint64_t last_fb_time;
}RtpRtxItem;

/* lijinglai add for rtprtx e */

/**
 * An object representing a bi-directional RTP session.
 * It holds sockets, jitter buffer, various counters (timestamp, sequence numbers...)
 * Applications SHOULD NOT try to read things within the RtpSession object but use
 * instead its public API (the rtp_session_* methods) where RtpSession is used as a 
 * pointer.
 * rtp_session_new() allocates and initialize a RtpSession.
**/
struct _RtpSession
{
	RtpSession *next;	/* next RtpSession, when the session are enqueued by the scheduler */
	int mask_pos;	/* the position in the scheduler mask of RtpSession : do not move this field: it is part of the ABI since the session_set macros use it*/
        struct {
	  RtpProfile *profile;
	  int pt;
	  unsigned int ssrc;
	  int telephone_events_pt;	/* the payload type used for telephony events */
	} snd,rcv;
	unsigned int inc_ssrc_candidate;
	int inc_same_ssrc_count;
	int hw_recv_pt; /* recv payload type before jitter buffer */
	int recv_buf_size;
	mblk_t *recv_buff;          /* added by tanghailang */
	RtpSignalTable on_ssrc_changed;
	RtpSignalTable on_payload_type_changed;
	RtpSignalTable on_telephone_event_packet;
	RtpSignalTable on_telephone_event;
	RtpSignalTable on_timestamp_jump;
	RtpSignalTable on_network_error;
	RtpSignalTable on_rtcp_bye;
	/* added by lwx201687 20160523 begin */
	RtpSignalTable on_reprot_lr;
	/* added by lwx201687 20160523 end */
	struct _OList *signal_tables;
	struct _OList *eventqs;
	msgb_allocator_t allocator;
	RtpStream rtp;
	RtcpStream rtcp;
  struct _OList *modules_tables;
	RtpSessionMode mode;
	struct _RtpScheduler *sched;
	uint32_t flags;
	int dscp;
	struct _tc_qos tc_qos;
	int multicast_ttl;
	int multicast_loopback;
	void * user_data;
	/* FIXME: Should be a table for all session participants. */
	struct timeval last_recv_time; /* Time of receiving the RTP/RTCP packet. */
	mblk_t *pending;
	/* telephony events extension */
	mblk_t *current_tev;		/* the pending telephony events */
	mblk_t *sd;
	queue_t contributing_sources;
	bool_t symmetric_rtp;
	bool_t permissive; /*use the permissive algorithm*/
	bool_t use_connect; /* use connect() on the socket */
	bool_t ssrc_set;
	bool_t rtcp_mux;
  void *tsc_handle;
  unsigned int tsc_redundancy_factor;
  int tsc_redundancy_lb;
  struct IceCheckList *rtp_icechecklist;
  struct IceCheckList *rtcp_icechecklist;
  uint64_t last_rectime;

  /* lijinglai add for rtprtx b */
  struct _OList *rtprtx_tables;
  uint16_t *fb_tables;
  uint32_t  fbnum;
  uint32_t  rtprtx_ssrc;
  int rtprtx_enable;
  uint64_t rtt;
  uint64_t rtprtx_interval;
  uint64_t rtprtx_check_time;
  uint16_t last_check_rtpseq;
  uint16_t rcv_first_rtprtx_num;
  uint32_t  congest_flag;
  uint32_t congest_thd;
  uint32_t congestdelay_thd;
  uint32_t congest_timer_thd;
  uint32_t congestdelay_timer_thd;
  uint32_t max_trans_time;
  int64_t  sumjitter;
  uint64_t congest_timer;
  uint64_t congest_delay_timer;
  /* lijinglai add for rtprtx e */
  
};

#ifdef __cplusplus
extern "C"
{
#endif

/* public API */
RtpSession *rtp_session_new(int mode);
void rtp_session_set_scheduling_mode(RtpSession *session, int yesno);
void rtp_session_set_blocking_mode(RtpSession *session, int yesno);
void rtp_session_set_profile(RtpSession *session, RtpProfile *profile);
void rtp_session_set_send_profile(RtpSession *session,RtpProfile *profile);
void rtp_session_set_recv_profile(RtpSession *session,RtpProfile *profile);
RtpProfile *rtp_session_get_profile(RtpSession *session);
RtpProfile *rtp_session_get_send_profile(RtpSession *session);
RtpProfile *rtp_session_get_recv_profile(RtpSession *session);
int rtp_session_signal_connect(RtpSession *session,const char *signal_name, RtpCallback cb, uintptr_t user_data);
int rtp_session_signal_disconnect_by_callback(RtpSession *session,const char *signal_name, RtpCallback cb);
void rtp_session_set_ssrc(RtpSession *session, uint32_t ssrc);
void rtp_session_set_seq_number(RtpSession *session, uint16_t seq);
uint16_t rtp_session_get_seq_number(RtpSession *session);

/* lijinglai add for rtprtx b */
void rtp_session_set_sum_jitter_parmeter(RtpSession *session, int value);
/* lijinglai add for rtprtx e */

void rtp_session_enable_jitter_buffer(RtpSession *session , bool_t enabled);
bool_t rtp_session_jitter_buffer_enabled(const RtpSession *session);
void rtp_session_set_jitter_buffer_params(RtpSession *session, const JBParameters *par);
void rtp_session_get_jitter_buffer_params(RtpSession *session, JBParameters *par);
float rtp_session_get_average_buffer_size(RtpSession *session);

/*deprecated jitter control functions*/
void rtp_session_set_jitter_compensation(RtpSession *session, int milisec);
void rtp_session_enable_adaptive_jitter_compensation(RtpSession *session, bool_t val);
bool_t rtp_session_adaptive_jitter_compensation_enabled(RtpSession *session);

void rtp_session_set_time_jump_limit(RtpSession *session, int miliseconds);
int rtp_session_set_local_addr(RtpSession *session,const char *addr, int port);
int rtp_session_get_local_port(const RtpSession *session);
int rtp_session_set_tsc_server(RtpSession *session, void *tunnel_handle, unsigned int tsc_redundancy_factor, int tsc_redundancy_lb);

int
rtp_session_set_remote_addr_full (RtpSession * session, const char * addr, int rtp_port, int rtcp_port);
/*same as previous function, old name:*/
int rtp_session_set_remote_addr_and_port (RtpSession * session, const char * addr, int rtp_port, int rtcp_port);
int rtp_session_set_remote_addr(RtpSession *session,const char *addr, int port);
/* alternatively to the set_remote_addr() and set_local_addr(), an application can give
a valid socket (potentially connect()ed )to be used by the RtpSession */
void rtp_session_set_sockets(RtpSession *session, int rtpfd, int rtcpfd);
void rtp_session_set_transports(RtpSession *session, RtpTransport *rtptr);
void rtp_session_add_module(RtpSession *session, RtpModule *module);
RtpModule *rtp_session_get_module(RtpSession *session, const char *name);
int rtp_session_remove_module(RtpSession *session, const char *name);
void rtp_session_module_remove_all(RtpSession *session);

/*those methods are provided for people who wants to send non-RTP messages using the RTP/RTCP sockets */
ortp_socket_t rtp_session_get_rtp_socket(const RtpSession *session);
ortp_socket_t rtp_session_get_rtcp_socket(const RtpSession *session);


/* QOS / DSCP */
int rtp_session_set_dscp(RtpSession *session, int dscp);
int rtp_session_get_dscp(const RtpSession *session);


/* Multicast methods */
int rtp_session_set_multicast_ttl(RtpSession *session, int ttl);
int rtp_session_get_multicast_ttl(RtpSession *session);

int rtp_session_set_multicast_loopback(RtpSession *session, int yesno);
int rtp_session_get_multicast_loopback(RtpSession *session);



int rtp_session_set_send_payload_type(RtpSession *session, int paytype);
int rtp_session_get_send_payload_type(const RtpSession *session);

int rtp_session_get_recv_payload_type(const RtpSession *session);
int rtp_session_set_recv_payload_type(RtpSession *session, int pt);

int rtp_session_set_payload_type(RtpSession *session, int pt);

void rtp_session_set_symmetric_rtp (RtpSession * session, bool_t yesno);

void rtp_session_set_rtcp_mux (RtpSession * session, bool_t yesno);

void rtp_session_set_connected_mode(RtpSession *session, bool_t yesno);

void rtp_session_enable_rtcp(RtpSession *session, bool_t yesno);

void rtp_session_set_ssrc_changed_threshold(RtpSession *session, int numpackets);

/*low level recv and send functions */
mblk_t * rtp_session_recvm_with_ts (RtpSession * session, uint32_t user_ts);
mblk_t * rtp_session_recvm_with_ts_extended (RtpSession * session, uint32_t user_ts, mblk_t **last_dropped);
mblk_t * rtp_session_create_packet(RtpSession *session,int header_size, const uint8_t *payload, int payload_size);
mblk_t * rtp_session_create_packet_with_data(RtpSession *session, uint8_t *payload, int payload_size, void (*freefn)(void*));
mblk_t * rtp_session_create_packet_in_place(RtpSession *session,uint8_t *buffer, int size, void (*freefn)(void*) );
int rtp_session_sendm_with_ts (RtpSession * session, mblk_t *mp, uint32_t userts);
int __rtp_session_sendm_with_ts (RtpSession * session, mblk_t *mp, uint32_t packet_ts, uint32_t send_ts); /* usefull for test purpose */
/* high level recv and send functions */
int rtp_session_recv_with_ts(RtpSession *session, uint8_t *buffer, int len, uint32_t ts, int *have_more);
int rtp_session_send_with_ts(RtpSession *session, const uint8_t *buffer, int len, uint32_t userts);

/* send data without applying module */
int rtp_session_data_send (RtpSession * session, mblk_t * m);

/* event API*/
void rtp_session_register_event_queue(RtpSession *session, OrtpEvQueue *q);
void rtp_session_unregister_event_queue(RtpSession *session, OrtpEvQueue *q);


/* IP bandwidth usage estimation functions, returning bits/s*/
float rtp_session_compute_send_bandwidth(RtpSession *session);
float rtp_session_compute_recv_bandwidth(RtpSession *session);
float rtp_session_compute_average_jitter(RtpSession *session);

/* added by tanghailang for lossrate calculation */
int rtp_session_get_lossrate(RtpSession *session);
/* added by tanghailang for framerate calculation */
int rtp_session_get_framerate(RtpSession *session);

void rtp_session_send_rtcp_APP(RtpSession *session, uint8_t subtype, const char *name, const uint8_t *data, int datalen);

uint32_t rtp_session_get_current_send_ts(RtpSession *session);
uint32_t rtp_session_get_current_recv_ts(RtpSession *session);
void rtp_session_flush_sockets(RtpSession *session);
void rtp_session_release_sockets(RtpSession *session);
void rtp_session_resync(RtpSession *session);
void rtp_session_reset(RtpSession *session);
void rtp_session_destroy(RtpSession *session);

const rtp_stats_t * rtp_session_get_stats(const RtpSession *session);
void rtp_session_reset_stats(RtpSession *session);

void rtp_session_set_data(RtpSession *session, void *data);
void *rtp_session_get_data(const RtpSession *session);

/* lijinglai add for rtprtx b */
void rtp_session_set_rtprtx_enable(RtpSession *session, int enable);
void rtp_session_set_rtprtx_ssrc(RtpSession *session, uint32_t rtprtx_ssrc);
void rtp_session_set_rtprtx_congestthd(RtpSession *session, uint32_t congestthd);
void rtp_session_set_rtprtx_congest_timerthd(RtpSession *session, uint32_t timerthd);
void rtp_session_set_rtprtx_congestdelaythd(RtpSession *session, uint32_t congestthd);
void rtp_session_set_rtprtx_congestdelay_timerthd(RtpSession *session, uint32_t timerthd);
void rtp_session_set_rtprtx_max_trans_time(RtpSession *session, uint32_t transtime);
void rtp_session_set_rtprtx_max_check_time(RtpSession *session, uint32_t checktime);
/* lijinglai add for rtprtx e */

void rtp_session_set_recv_buf_size(RtpSession *session, int bufsize);
void rtp_session_set_rtp_socket_send_buffer_size(RtpSession * session, unsigned int size);
void rtp_session_set_rtp_socket_recv_buffer_size(RtpSession * session, unsigned int size);
 
/* in use with the scheduler to convert a timestamp in scheduler time unit (ms) */
uint32_t rtp_session_ts_to_time(RtpSession *session,uint32_t timestamp);
uint32_t rtp_session_time_to_ts(RtpSession *session, int millisecs);
/* this function aims at simulating senders with "imprecise" clocks, resulting in 
rtp packets sent with timestamp uncorrelated with the system clock .
This is only availlable to sessions working with the oRTP scheduler */
void rtp_session_make_time_distorsion(RtpSession *session, int milisec);

int rtp_session_set_icechecklist (RtpSession * session, struct IceCheckList *rtp_checklist, struct IceCheckList *rtcp_checklist);

/*RTCP functions */
void rtp_session_set_source_description(RtpSession *session, const char *cname,
	const char *name, const char *email, const char *phone, 
    const char *loc, const char *tool, const char *note);
void rtp_session_add_contributing_source(RtpSession *session, uint32_t csrc, 
    const char *cname, const char *name, const char *email, const char *phone, 
    const char *loc, const char *tool, const char *note);
void rtp_session_remove_contributing_sources(RtpSession *session, uint32_t csrc);
mblk_t* rtp_session_create_rtcp_sdes_packet(RtpSession *session);

void rtp_session_get_last_recv_time(RtpSession *session, struct timeval *tv);
int rtp_session_bye(RtpSession *session, const char *reason);

int rtp_session_get_last_send_error_code(RtpSession *session);
void rtp_session_clear_send_error_code(RtpSession *session);
int rtp_session_get_last_recv_error_code(RtpSession *session);
void rtp_session_clear_recv_error_code(RtpSession *session);

/*private */
void rtp_session_init(RtpSession *session, int mode);
#define rtp_session_set_flag(session,flag) (session)->flags|=(flag)
#define rtp_session_unset_flag(session,flag) (session)->flags&=~(flag)
void rtp_session_uninit(RtpSession *session);

void rtp_session_dispatch_event(RtpSession *session, OrtpEvent *ev);

#ifdef __cplusplus
}
#endif

#endif
