

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Apr 18 10:12:40 2017
 */
/* Compiler settings for eLTE_Player.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_eLTE_PlayerLib,0xEBB01215,0xC409,0x4D39,0xA1,0x5D,0x58,0x16,0xC3,0x85,0x2B,0x0B);


MIDL_DEFINE_GUID(IID, DIID__DeLTE_Player,0x76935AE5,0xA248,0x4C3E,0x87,0x59,0x00,0xB6,0x16,0xF9,0x7F,0x41);


MIDL_DEFINE_GUID(IID, DIID__DeLTE_PlayerEvents,0x241C1E26,0xBFDE,0x49BC,0xA4,0x40,0xE6,0xC2,0x8D,0xF3,0xA7,0xEE);


MIDL_DEFINE_GUID(CLSID, CLSID_eLTE_Player,0x6AB35B6F,0xD5D4,0x4DA3,0x95,0x03,0x1A,0xD4,0xE5,0xE1,0xE5,0x89);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



