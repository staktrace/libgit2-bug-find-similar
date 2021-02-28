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

//	CLibMsgPeriodical.cp

#include "CLibMsgPeriodical.h"
#include "msgcom.h"

// ���������������������������������������������������������������������������
//	���
//	�	Class CLibMsgPeriodical
//	���
//
//	This is a subclass of LPeriodical which calls the new libmsg init, exit,
//	and idle routines.
// ���������������������������������������������������������������������������

CLibMsgPeriodical::CLibMsgPeriodical() :
LPeriodical()
{
	MSG_InitMsgLib();
	
	/* use less memory! */
	MSG_SetLibNeoCacheSize ( 512 * 1024L );
}

CLibMsgPeriodical::~CLibMsgPeriodical()
{
	MSG_ShutdownMsgLib();
}

void CLibMsgPeriodical::SpendTime(const EventRecord& /* inMacEvent */)
{
	MSG_OnIdle();
}