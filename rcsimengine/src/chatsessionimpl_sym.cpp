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


#include <e32base.h>
#include <e32std.h>

#include <mcemessagesource.h>
#include <mcemessagestream.h>

#include <sipprofileregistry.h>
#include <flogger.h>

#include "chatsessionimpl_sym.h"
#include "msrpchatsessiondata_sym.h"

using namespace RcsIMLib;

//_LIT( KLogDir1, "rcs" );
//_LIT( KLogFile1, "rcs.txt" );



ChatSessionImpl::ChatSessionImpl(ChatSession *apParent):mParent(apParent)
{

}

bool ChatSessionImpl::sendChatData(QString newChatData)
{
	TPtrC16 dataPtr(reinterpret_cast<const TUint16*>(newChatData.utf16()));
	RBuf8 chatData;
	chatData.Create(300);
	chatData.Copy(dataPtr);

	mMessageSource->SendDataL(chatData);

	chatData.Close();

	return true;
}


bool ChatSessionImpl::endChatSession()
{
	return true;
}

void ChatSessionImpl::setPlatformParams(void *apPlatFormParam)
{
	mMsrpChatSessionData = static_cast<TMsrpChatSession*>(apPlatFormParam);
    if (mMsrpChatSessionData->iStream1->Source()->Type() == KMceMessageSource)
    {
    	mMessageSource = (CMceMessageSource*) mMsrpChatSessionData->iStream1->Source();
    }
    else if (mMsrpChatSessionData->iStream2->Source()->Type() == KMceMessageSource)
    {
        mMessageSource = (CMceMessageSource*) mMsrpChatSessionData->iStream2->Source();
    }
}

//Get the Platform implementations
void* ChatSessionImpl::getPlatformImpl()
{
	return this;
}

void ChatSessionImpl::incomingData(const TDesC8& aData)
{
    QString chatData = QString::fromUtf8((const char*)aData.Ptr(),aData.Length());
    emit mParent->newChatData(chatData);
}
