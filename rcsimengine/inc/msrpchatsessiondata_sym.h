/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html."
* Initial Contributors:
* Nokia Corporation - initial contribution.
* Contributors:
*
* Description:
* RCS IM Library - Initial version
*
*/


#ifndef __MSRP_CHATSESSIONDATA_SYM_H__
#define __MSRP_CHATSESSIONDATA_SYM_H__

//#include "rcschatengine_globals.h"

class CMceMediaStream;
class CMceSession;
class CMceMediaStream;
class ChatSession;

class TMsrpChatSession
{
	public:
	CMceManager *iMceManager;
	CMceSession *iMceSession;
	ChatSession *iChatSession;
	CMceMediaStream *iStream1;
	CMceMediaStream *iStream2;
	QString          iInitMsg;
};

#endif //__MSRP_CHATSESSIONDATA_SYM_H__
