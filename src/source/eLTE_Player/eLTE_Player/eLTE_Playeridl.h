

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Feb 16 10:00:41 2017
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __eLTE_Playeridl_h__
#define __eLTE_Playeridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DeLTE_Player_FWD_DEFINED__
#define ___DeLTE_Player_FWD_DEFINED__
typedef interface _DeLTE_Player _DeLTE_Player;
#endif 	/* ___DeLTE_Player_FWD_DEFINED__ */


#ifndef ___DeLTE_PlayerEvents_FWD_DEFINED__
#define ___DeLTE_PlayerEvents_FWD_DEFINED__
typedef interface _DeLTE_PlayerEvents _DeLTE_PlayerEvents;
#endif 	/* ___DeLTE_PlayerEvents_FWD_DEFINED__ */


#ifndef __eLTE_Player_FWD_DEFINED__
#define __eLTE_Player_FWD_DEFINED__

#ifdef __cplusplus
typedef class eLTE_Player eLTE_Player;
#else
typedef struct eLTE_Player eLTE_Player;
#endif /* __cplusplus */

#endif 	/* __eLTE_Player_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __eLTE_PlayerLib_LIBRARY_DEFINED__
#define __eLTE_PlayerLib_LIBRARY_DEFINED__

/* library eLTE_PlayerLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_eLTE_PlayerLib;

#ifndef ___DeLTE_Player_DISPINTERFACE_DEFINED__
#define ___DeLTE_Player_DISPINTERFACE_DEFINED__

/* dispinterface _DeLTE_Player */
/* [uuid] */ 


EXTERN_C const IID DIID__DeLTE_Player;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("76935AE5-A248-4C3E-8759-00B616F97F41")
    _DeLTE_Player : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DeLTE_PlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DeLTE_Player * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DeLTE_Player * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DeLTE_Player * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DeLTE_Player * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DeLTE_Player * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DeLTE_Player * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DeLTE_Player * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DeLTE_PlayerVtbl;

    interface _DeLTE_Player
    {
        CONST_VTBL struct _DeLTE_PlayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DeLTE_Player_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DeLTE_Player_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DeLTE_Player_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DeLTE_Player_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DeLTE_Player_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DeLTE_Player_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DeLTE_Player_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DeLTE_Player_DISPINTERFACE_DEFINED__ */


#ifndef ___DeLTE_PlayerEvents_DISPINTERFACE_DEFINED__
#define ___DeLTE_PlayerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DeLTE_PlayerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DeLTE_PlayerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("241C1E26-BFDE-49BC-A440-E6C28DF3A7EE")
    _DeLTE_PlayerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DeLTE_PlayerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DeLTE_PlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DeLTE_PlayerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DeLTE_PlayerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DeLTE_PlayerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DeLTE_PlayerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DeLTE_PlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DeLTE_PlayerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DeLTE_PlayerEventsVtbl;

    interface _DeLTE_PlayerEvents
    {
        CONST_VTBL struct _DeLTE_PlayerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DeLTE_PlayerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DeLTE_PlayerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DeLTE_PlayerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DeLTE_PlayerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DeLTE_PlayerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DeLTE_PlayerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DeLTE_PlayerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DeLTE_PlayerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_eLTE_Player;

#ifdef __cplusplus

class DECLSPEC_UUID("6AB35B6F-D5D4-4DA3-9503-1AD4E5E1E589")
eLTE_Player;
#endif
#endif /* __eLTE_PlayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


