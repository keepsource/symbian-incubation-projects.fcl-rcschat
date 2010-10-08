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


#include "chatcontactmanager.h"

#ifdef Q_OS_SYMBIAN
#include "chatcontactmanagerimpl_sym.h"
#endif

using namespace RcsIMLib;

ChatContactManager::ChatContactManager()
{
	mpImpl = new ChatContactManagerImpl(this);
}

ChatContactManager::~ChatContactManager()
    {
    delete mpImpl;
    }

RcsIMLib::RcsChatId ChatContactManager::createChatSession(QContactLocalId contactId, QString initMsg)
{
    return mpImpl->createChatSession(contactId, initMsg);
}

void ChatContactManager::closeSession(RcsIMLib::RcsChatId sessID)
{
    mpImpl->closeSession(sessID);
}

void ChatContactManager::acceptIncomingSession(RcsIMLib::RcsChatId sessID)
{
    mpImpl->acceptIncomingSession(sessID);
}
