/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef HOST_DEC_H
#define HOST_DEC_H

#include "types.h"
#include "host_types.h"
#include "host_hamac_pipe.h"
#include "host_input_control.h"

#ifndef OMX_TICKS_PER_SECOND
#ifdef __SYMBIAN32__
typedef unsigned long long OMX_U64;
typedef signed long long OMX_S64;
#elif defined(WIN32)
typedef unsigned __int64  OMX_U64;
typedef signed   __int64  OMX_S64;
#else /* WIN32 */
typedef unsigned long long OMX_U64;
typedef signed long long OMX_S64;
#endif /* WIN32 */
#define OMX_TICKS_PER_SECOND 1000000
#endif

typedef struct t_picture_parameter_type
{
	t_uint16 par_width;
	t_uint16 par_height;
	t_uint16 frame_cropping_flag;
	t_uint16 frame_crop_right;
	t_uint16 frame_crop_left;
	t_uint16 frame_crop_top;
	t_uint16 frame_crop_bottom;
	t_uint16 colorspace;
} t_picture_parameter;

typedef struct t_dec_type
{
 	t_uint16 	sh_status;
	t_uint16	nut;
	t_uint16	stop;
	t_sint16 	num_seq_par;
	t_sint16  num_subset_seq_par;
	t_sint16 	num_pic_par;
	t_uint16 	next_idr;		/* next expected slice must be an IDR         */
	t_uint16 	failure; 		/* memorised if a problem has been encountered */
    t_uint16 	first_slice;
    t_uint16 	sps_active ;
    t_uint16 	error_count;
    t_uint16 	use_rs;


	t_uint16	maxslices;
    t_sint16 	slice_num;
	t_seq_par stab[32];
    t_pic_par ptab[256];
    t_sint32 	nframes;
    t_slice_hdr *prevSh;
    t_slice_hdr old_sh;
	t_seq_par 	*p_active_sp;
    t_seq_par 	old_sp;
    t_pic_par 	*p_active_pp;
    t_slice_hdr *sh;
    t_old_slice old_sl;
    t_host_info host;
    t_dec_buff  buf;

    t_seq_par tmp_sps;
    t_seq_par tmp_sps2; /* for subset SPS */
    t_slice_hdr tmp_sh; /* for dummy grey image */
    t_uint16  pending_sps_update;

    t_subset_seq_par subset_sp[32];
    t_subset_seq_par *p_active_subset_sp;
    t_subset_seq_par old_subset_sp;

  t_uint32 aux_counter;     /* for synchro between hamac pipe and aux hamac pipe */
	t_input_control input_control;
	t_hamac_pipe hamac_pipe;
    t_uint16 DBLK_mode;
								/**<\brief DBLK_mode == 0 : No deblocking (no more bit true )   *
                                   *        DBLK_mode == 1 : Deblocking at the end of decode     *
                                   *        DBLK_mode == 3 : Deblocking parallelized with decode *
                                   *                         ( possible when no FMO )            */
    t_uint16 sva_bypass;
								/**<\brief sva_bypass == 0 : No bypass *
                                   *        sva_bypass == 1 : No dsp call *
                                   *        sva_bypass == 3 : No codec call on DSP, still ddep call */
	t_picture_parameter picture_parameters;
	t_uint16 gone_to_sleep;
	t_uint16 esram_allocated;
	t_uint16 is_mvc_stream;
	t_uint16 is_first_sei_chunk;

	t_uint16 bThumbnailGeneration;
        t_uint16 bImmediateRelease ;
	t_uint16 restrictMaxLevel;
	t_uint16 isSoc1080pCapable;

	/* for output timestamps management */
	OMX_S64  g_last_timestamp;   /* keeps track of timestamp of last picture, to detect if TS are monotonically increasing */
	OMX_S64  g_start_time;       /* each time a seek happens, a flush is done and the first NAL after flush will contain the new position in the media file */
  t_uint32 g_first_nal_after_flush; /* to be able to detect first NAL after a flush and memorize start time */
  t_uint32 g_wrong_timestamps; /* flag telling that timestamps from demuxer are wrong and must be auto-generated by decoder */
  t_uint32 g_omx_framerate;    /* info extracted from container */
  t_uint32 g_no_timestamps_from_demuxer; /* detect the case where demuxer provides no timestamps at all */

}
t_dec;

t_uint16	init_decoder(void);
void 	close_decoder(void);

void HamacFillParamIn(void *vfm_memory_context,t_dec *dec, t_uint16 nslices,t_hamac_param *t);
void HamacAllocParameter(void *vfm_memory_context,t_hamac_param *t,t_uint16 n);
void HamacDeallocParameter(void *vfm_memory_context,t_hamac_param *t, t_uint16 n);
void HamacToPhysicalAddresses(void *vfm_memory_context,t_hamac_param *t, t_uint16 nslices);
void HamacToLogicalAddresses(void *vfm_memory_context,t_hamac_param *t, t_uint16 nslices);

#endif