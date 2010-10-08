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


#ifndef __CHATSESSIONMANAGER_H__
#define __CHATSESSIONMANAGER_H__

#include "chatinterfaces.h"
//#include "rcschatengine_globals.h"

class ChatSessionImpl;

class ChatSession:public RcsIMLib::MChatSessionIntf
{
	Q_OBJECT
	public:
	ChatSession();
	
	
	/* Close the session */
	bool endChatSession();
   	bool sendChatData(QString newChatData);

   	
	void setPlatformParams(void *apPlatFormParam);

	//Get the Platform implementations
	void* getPlatformImpl();
	
	
	private:
	ChatSessionImpl *mpImpl;	
	friend class ChatSessionImpl;
};

#endif

