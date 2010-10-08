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


#include "chatsession.h"
#ifdef Q_OS_SYMBIAN
#include "chatsessionimpl_sym.h"
#endif

using namespace RcsIMLib;

ChatSession::ChatSession()
{
	mpImpl = new ChatSessionImpl(this);
}

bool ChatSession::endChatSession()
{
	return mpImpl->endChatSession();
}

bool ChatSession::sendChatData(QString newChatData)
{
    return mpImpl->sendChatData(newChatData);
}

void ChatSession::setPlatformParams(void *apPlatFormParam)
{
	mpImpl->setPlatformParams(apPlatFormParam);
}

//Get the Platform implementations
void* ChatSession::getPlatformImpl()
{
	return mpImpl->getPlatformImpl();
}
