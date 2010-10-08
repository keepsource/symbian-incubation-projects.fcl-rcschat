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


#ifndef __CHATCONTACT_MANAGER_H__
#define __CHATCONTACT_MANAGER_H__

#include "chatinterfaces.h"
//#include "rcschatengine_globals.h"

class ChatContactManagerImpl;

/* Interface implementation of MChatContactIntf
 */
class ChatContactManager: public RcsIMLib::MChatContactManagerIntf
{
	Q_OBJECT
	public:
		ChatContactManager();
		~ChatContactManager();
		RcsIMLib::RcsChatId createChatSession(QContactLocalId contactId, QString initMsg);
		void closeSession(RcsIMLib::RcsChatId sessID);
		void acceptIncomingSession(RcsIMLib::RcsChatId sessID);
		
	
	private:
		ChatContactManagerImpl *mpImpl;
		friend class ChatContactManagerImpl;
};

#endif //__CHATCONTACT_MANAGER_H__
