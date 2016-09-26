/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef rfc3984_h
#define rfc3984_h

#include "mediastreamer2/mscommon.h"
#include "mediastreamer2/msqueue.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Rfc3984Context{
	MSQueue q;
	mblk_t *m;
	int maxsz;
	uint32_t last_ts;
	uint32_t last_cseq;
	uint8_t mode;
	bool_t stap_a_allowed;
	msgb_allocator_t allocator;  /*added by tanghailang*/
	uint8_t reserved;

	/*added by lwx201687 begin*/
	int32_t continu_p_frames;
	int32_t min_drop_frame_number;
	int32_t max_frame_number;
	bool_t frame_gap_allowed;
	bool_t drop_p_frame;
	bool_t  idr_frame_broken;
	bool_t  printf_broken_frame;
	int32_t frame_number;
	uint32_t last_unit_ts;
	
	/*added by lwx201687 end*/

	/* added by lwx201687 20160523 begin */
	int32_t video_loassrate;
	int32_t video_min_loassrate;
	/* added by lwx201687 20160523 end */

	/* added by lwx201687 20160613 begin */
	bool_t  idr_frame_gap_bigger;
	int32_t  bigger_frame_num;
	uint64_t rcv_first_idr_time;
	/* added by lwx201687 20160613 end */
	
} Rfc3984Context;

void rfc3984_init(Rfc3984Context *ctx);

void rfc3984_set_max_payload_size(Rfc3984Context *ctx, int size);
	
void rfc3984_set_mode(Rfc3984Context *ctx, int mode);

/* some stupid phones don't decode STAP-A packets ...*/
void rfc3984_enable_stap_a(Rfc3984Context *ctx, bool_t yesno);

/*process NALUs and pack them into rtp payloads */
void rfc3984_pack(Rfc3984Context *ctx, MSQueue *naluq, MSQueue *rtpq, uint32_t ts);

/*process incoming rtp data and output NALUs, whenever possible*/
void rfc3984_unpack(Rfc3984Context *ctx, mblk_t *im, MSQueue *naluq);

void rfc3984_uninit(Rfc3984Context *ctx);

#ifdef __cplusplus
}
#endif

#endif

