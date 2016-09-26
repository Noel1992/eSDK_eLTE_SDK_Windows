/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)
Copyright (C) 2012  Aymeric Moizard (amoizard@gmail.com)

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

#ifndef msplc_h
#define msplc_h

#include "mscommon.h"

/**
* @file msplc.h
* @brief mediastreamer2 msplc.h include file
*
* This file provide the API needed to manage
* PLC.
*
*/

/**
* @defgroup mediastreamer2_plc PLC plugin API - manage audio plc.
* @ingroup mediastreamer2_api
* @{
*/

struct _MSPlc {
  void *data;
  int samplerate;
};

/**
* Structure for PLC object.
* @var MSPlc
*/
typedef struct _MSPlc MSPlc;

typedef int (*ms_plc_initFunc)(MSPlc *obj);
typedef void (*ms_plc_freeFunc)(MSPlc *obj);
typedef int (*ms_plc_expandFunc)(MSPlc *obj, mblk_t  **out, int length, int noise);
typedef int (*ms_plc_normalFunc)(MSPlc *obj, mblk_t  *in, mblk_t  **out);
typedef int (*ms_plc_reduceFunc)(MSPlc *obj, mblk_t  *in, mblk_t  **out);

struct _MSPlcDesc {
  int struct_size;
  char name[32];
  int quality_priority;
  ms_plc_initFunc plc_init;
  ms_plc_freeFunc plc_free;
  ms_plc_expandFunc plc_expand;
  ms_plc_normalFunc plc_normal;
  ms_plc_reduceFunc plc_reduce;
};

/**
* Structure for PLC plugin.
* @var MSPlcDesc
*/
typedef struct _MSPlcDesc MSPlcDesc;

#ifdef __cplusplus
extern "C"{
#endif

  /**
  * Set PLC plugin description.
  *
  * @param _ms_plc_desc      A PLC plugin description.
  */
  void ms_plc_set_func(MSPlcDesc *_ms_plc_desc);

  /**
  * Retreive a PLC object.
  *
  * @param samplerate        samplerate for PLC.
  *
  * Returns: PLC object if successful, NULL otherwise.
  */
  MSPlc *ms_plc_init(int samplerate);

  /**
  * Destroy a PLC object.
  *
  * @param obj           A PLC object.
  */
  void ms_plc_free(MSPlc *obj);

  /**
  * Expand audio.
  *
  * @param obj           A PLC object.
  * @param out           output audio.
  * @param length        length of voice in unsigned char
  * @param noise         0 if packet loss, 1 if reducing to noise
  * 
  * Returns: 0 if successful, negative otherwise.
  */
  int ms_plc_expand(MSPlc *obj, mblk_t  **out, int length, int noise);

  /**
  * Expand audio.
  *
  * @param obj           A PLC object.
  * @param in            input audio.
  * @param out           output audio.
  * 
  * Returns: 0 if successful, negative otherwise.
  */
  int ms_plc_normal(MSPlc *obj, mblk_t  *in, mblk_t  **out);

  /**
  * Expand audio.
  *
  * @param obj           A PLC object.
  * @param in            input audio.
  * @param out           output audio.
  * 
  * Returns: 0 if successful, negative otherwise.
  */
  int ms_plc_reduce(MSPlc *obj, mblk_t  *in, mblk_t  **out);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
