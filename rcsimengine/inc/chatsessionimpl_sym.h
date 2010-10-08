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


#ifndef __CHATSESSION_IMPL_SYM_H__
#define __CHATSESSION_IMPL_SYM_H__

#include <QTimer>
#include "chatinterfaces.h"
#include "chatsession.h"
//#include "rcschatengine_globals.h"

class TMsrpChatSession;
class CMceMessageSource;
class TDesC8;


class ChatSessionImpl: public QObject
{
	Q_OBJECT
	public:
	ChatSessionImpl(ChatSession *apParent);
   	
	/* Close the session */
    bool endChatSession();
    
    bool sendChatData(QString newChatData);

    
    //****************
	void setPlatformParams(void *apPlatFormParam);

	//Get the Platform implementations
	void* getPlatformImpl();
	void  incomingData(const TDesC8& aData);


	ChatSession *mParent;
	TMsrpChatSession* mMsrpChatSessionData;
	CMceMessageSource* mMessageSource;
	

};

#endif

