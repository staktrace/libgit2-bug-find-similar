/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
/*******************************************************************************
 * Source date: 16 Apr 1997 08:37:58 GMT
 * netscape/fonts/nffbu public interface
 * Generated by jmc version 1.8 -- DO NOT EDIT
 ******************************************************************************/

#ifndef _Mnffbu_H_
#define _Mnffbu_H_

#include "jmc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * nffbu
 ******************************************************************************/

/* The type of the nffbu interface. */
struct nffbuInterface;

/* The public type of a nffbu instance. */
typedef struct nffbu {
	const struct nffbuInterface*	vtable;
} nffbu;

/* The inteface ID of the nffbu interface. */
#ifndef JMC_INIT_nffbu_ID
extern EXTERN_C_WITHOUT_EXTERN const JMCInterfaceID nffbu_ID;
#else
EXTERN_C const JMCInterfaceID nffbu_ID = { 0x54743863, 0x7f213574, 0x1003351a, 0x07314250 };
#endif /* JMC_INIT_nffbu_ID */
/*******************************************************************************
 * nffbu Operations
 ******************************************************************************/

#define nffbu_getInterface(self, a, exception)	\
	(((self)->vtable->getInterface)(self, nffbu_getInterface_op, a, exception))

#define nffbu_addRef(self, exception)	\
	(((self)->vtable->addRef)(self, nffbu_addRef_op, exception))

#define nffbu_release(self, exception)	\
	(((self)->vtable->release)(self, nffbu_release_op, exception))

#define nffbu_hashCode(self, exception)	\
	(((self)->vtable->hashCode)(self, nffbu_hashCode_op, exception))

#define nffbu_equals(self, a, exception)	\
	(((self)->vtable->equals)(self, nffbu_equals_op, a, exception))

#define nffbu_clone(self, exception)	\
	(((self)->vtable->clone)(self, nffbu_clone_op, exception))

#define nffbu_toString(self, exception)	\
	(((self)->vtable->toString)(self, nffbu_toString_op, exception))

#define nffbu_finalize(self, exception)	\
	(((self)->vtable->finalize)(self, nffbu_finalize_op, exception))

#define nffbu_CreateFontMatchInfo(self, a, b, c, d, e, f, g, h, i, j, exception)	\
	(((self)->vtable->CreateFontMatchInfo)(self, nffbu_CreateFontMatchInfo_op, a, b, c, d, e, f, g, h, i, j, exception))

#define nffbu_CreateRenderingContext(self, a, b, c, c_length, exception)	\
	(((self)->vtable->CreateRenderingContext)(self, nffbu_CreateRenderingContext_op, a, b, c, c_length, exception))

#define nffbu_CreateFontObserver(self, a, b, exception)	\
	(((self)->vtable->CreateFontObserver)(self, nffbu_CreateFontObserver_op, a, b, exception))

#define nffbu_malloc(self, a, exception)	\
	(((self)->vtable->malloc)(self, nffbu_malloc_op, a, exception))

#define nffbu_free(self, a, exception)	\
	(((self)->vtable->free)(self, nffbu_free_op, a, exception))

#define nffbu_realloc(self, a, b, exception)	\
	(((self)->vtable->realloc)(self, nffbu_realloc_op, a, b, exception))

#define nffbu_IsWebfontsEnabled(self, exception)	\
	(((self)->vtable->IsWebfontsEnabled)(self, nffbu_IsWebfontsEnabled_op, exception))

#define nffbu_EnableWebfonts(self, exception)	\
	(((self)->vtable->EnableWebfonts)(self, nffbu_EnableWebfonts_op, exception))

#define nffbu_DisableWebfonts(self, exception)	\
	(((self)->vtable->DisableWebfonts)(self, nffbu_DisableWebfonts_op, exception))

#define nffbu_ListFontDisplayers(self, exception)	\
	(((self)->vtable->ListFontDisplayers)(self, nffbu_ListFontDisplayers_op, exception))

#define nffbu_IsFontDisplayerEnabled(self, a, exception)	\
	(((self)->vtable->IsFontDisplayerEnabled)(self, nffbu_IsFontDisplayerEnabled_op, a, exception))

#define nffbu_ListFontDisplayersForMimetype(self, a, exception)	\
	(((self)->vtable->ListFontDisplayersForMimetype)(self, nffbu_ListFontDisplayersForMimetype_op, a, exception))

#define nffbu_FontDisplayerForMimetype(self, a, exception)	\
	(((self)->vtable->FontDisplayerForMimetype)(self, nffbu_FontDisplayerForMimetype_op, a, exception))

#define nffbu_EnableFontDisplayer(self, a, exception)	\
	(((self)->vtable->EnableFontDisplayer)(self, nffbu_EnableFontDisplayer_op, a, exception))

#define nffbu_DisableFontDisplayer(self, a, exception)	\
	(((self)->vtable->DisableFontDisplayer)(self, nffbu_DisableFontDisplayer_op, a, exception))

#define nffbu_EnableMimetype(self, a, b, exception)	\
	(((self)->vtable->EnableMimetype)(self, nffbu_EnableMimetype_op, a, b, exception))

#define nffbu_DisableMimetype(self, a, b, exception)	\
	(((self)->vtable->DisableMimetype)(self, nffbu_DisableMimetype_op, a, b, exception))

#define nffbu_LoadCatalog(self, a, exception)	\
	(((self)->vtable->LoadCatalog)(self, nffbu_LoadCatalog_op, a, exception))

#define nffbu_SaveCatalog(self, a, exception)	\
	(((self)->vtable->SaveCatalog)(self, nffbu_SaveCatalog_op, a, exception))

#define nffbu_LoadWebfont(self, a, b, c, exception)	\
	(((self)->vtable->LoadWebfont)(self, nffbu_LoadWebfont_op, a, b, c, exception))

#define nffbu_ReleaseWebfonts(self, a, exception)	\
	(((self)->vtable->ReleaseWebfonts)(self, nffbu_ReleaseWebfonts_op, a, exception))

#define nffbu_WebfontsNeedReload(self, a, exception)	\
	(((self)->vtable->WebfontsNeedReload)(self, nffbu_WebfontsNeedReload_op, a, exception))

#define nffbu_LookupFailed(self, a, b, c, exception)	\
	(((self)->vtable->LookupFailed)(self, nffbu_LookupFailed_op, a, b, c, exception))

#define nffbu_ToUnicode(self, a, b, b_length, c, c_length, exception)	\
	(((self)->vtable->ToUnicode)(self, nffbu_ToUnicode_op, a, b, b_length, c, c_length, exception))

#define nffbu_UnicodeLen(self, a, b, b_length, exception)	\
	(((self)->vtable->UnicodeLen)(self, nffbu_UnicodeLen_op, a, b, b_length, exception))

/*******************************************************************************
 * nffbu Interface
 ******************************************************************************/

struct netscape_jmc_JMCInterfaceID;
struct java_lang_Object;
struct java_lang_String;
struct netscape_jmc_ConstCString;
struct netscape_fonts_nffmi;
struct netscape_fonts_nfrc;
struct netscape_fonts_nfFontObserverCallback;
struct netscape_fonts_nfdoer;
struct netscape_fonts_MWContextStar;

struct nffbuInterface {
	void*	(*getInterface)(struct nffbu* self, jint op, const JMCInterfaceID* a, JMCException* *exception);
	void	(*addRef)(struct nffbu* self, jint op, JMCException* *exception);
	void	(*release)(struct nffbu* self, jint op, JMCException* *exception);
	jint	(*hashCode)(struct nffbu* self, jint op, JMCException* *exception);
	jbool	(*equals)(struct nffbu* self, jint op, void* a, JMCException* *exception);
	void*	(*clone)(struct nffbu* self, jint op, JMCException* *exception);
	const char*	(*toString)(struct nffbu* self, jint op, JMCException* *exception);
	void	(*finalize)(struct nffbu* self, jint op, JMCException* *exception);
	struct nffmi*	(*CreateFontMatchInfo)(struct nffbu* self, jint op, const char* a, const char* b, const char* c, jint d, jint e, jint f, jint g, jint h, jint i, jint j, JMCException* *exception);
	struct nfrc*	(*CreateRenderingContext)(struct nffbu* self, jint op, jint a, jint b, void** c, jsize c_length, JMCException* *exception);
	struct nfdoer*	(*CreateFontObserver)(struct nffbu* self, jint op, nfFontObserverCallback a, void* b, JMCException* *exception);
	void*	(*malloc)(struct nffbu* self, jint op, jint a, JMCException* *exception);
	void	(*free)(struct nffbu* self, jint op, void* a, JMCException* *exception);
	void*	(*realloc)(struct nffbu* self, jint op, void* a, jint b, JMCException* *exception);
	jint	(*IsWebfontsEnabled)(struct nffbu* self, jint op, JMCException* *exception);
	jint	(*EnableWebfonts)(struct nffbu* self, jint op, JMCException* *exception);
	jint	(*DisableWebfonts)(struct nffbu* self, jint op, JMCException* *exception);
	void*	(*ListFontDisplayers)(struct nffbu* self, jint op, JMCException* *exception);
	jint	(*IsFontDisplayerEnabled)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	void*	(*ListFontDisplayersForMimetype)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	const char*	(*FontDisplayerForMimetype)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	jint	(*EnableFontDisplayer)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	jint	(*DisableFontDisplayer)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	jint	(*EnableMimetype)(struct nffbu* self, jint op, const char* a, const char* b, JMCException* *exception);
	jint	(*DisableMimetype)(struct nffbu* self, jint op, const char* a, const char* b, JMCException* *exception);
	jint	(*LoadCatalog)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	jint	(*SaveCatalog)(struct nffbu* self, jint op, const char* a, JMCException* *exception);
	jint	(*LoadWebfont)(struct nffbu* self, jint op, MWContext * a, const char* b, jint c, JMCException* *exception);
	jint	(*ReleaseWebfonts)(struct nffbu* self, jint op, MWContext * a, JMCException* *exception);
	jint	(*WebfontsNeedReload)(struct nffbu* self, jint op, MWContext * a, JMCException* *exception);
	jint	(*LookupFailed)(struct nffbu* self, jint op, MWContext * a, struct nfrc* b, struct nffmi* c, JMCException* *exception);
	jint	(*ToUnicode)(struct nffbu* self, jint op, const char* a, jbyte* b, jsize b_length, jshort* c, jsize c_length, JMCException* *exception);
	jint	(*UnicodeLen)(struct nffbu* self, jint op, const char* a, jbyte* b, jsize b_length, JMCException* *exception);
};

/*******************************************************************************
 * nffbu Operation IDs
 ******************************************************************************/

typedef enum nffbuOperations {
	nffbu_getInterface_op,
	nffbu_addRef_op,
	nffbu_release_op,
	nffbu_hashCode_op,
	nffbu_equals_op,
	nffbu_clone_op,
	nffbu_toString_op,
	nffbu_finalize_op,
	nffbu_CreateFontMatchInfo_op,
	nffbu_CreateRenderingContext_op,
	nffbu_CreateFontObserver_op,
	nffbu_malloc_op,
	nffbu_free_op,
	nffbu_realloc_op,
	nffbu_IsWebfontsEnabled_op,
	nffbu_EnableWebfonts_op,
	nffbu_DisableWebfonts_op,
	nffbu_ListFontDisplayers_op,
	nffbu_IsFontDisplayerEnabled_op,
	nffbu_ListFontDisplayersForMimetype_op,
	nffbu_FontDisplayerForMimetype_op,
	nffbu_EnableFontDisplayer_op,
	nffbu_DisableFontDisplayer_op,
	nffbu_EnableMimetype_op,
	nffbu_DisableMimetype_op,
	nffbu_LoadCatalog_op,
	nffbu_SaveCatalog_op,
	nffbu_LoadWebfont_op,
	nffbu_ReleaseWebfonts_op,
	nffbu_WebfontsNeedReload_op,
	nffbu_LookupFailed_op,
	nffbu_ToUnicode_op,
	nffbu_UnicodeLen_op
} nffbuOperations;

/******************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _Mnffbu_H_ */
