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


#ifndef __CHATCONTACT_MANAGER_IMPL_SYM_H__
#define __CHATCONTACT_MANAGER_IMPL_SYM_H__


#include <QObject>
#include <QList>
#include "chatinterfaces.h"
#include "qmobilityglobal.h"
#include "qcontactfilter.h"
#include "qcontactdetailfilter.h"
#include "qtcontactsglobal.h"


//SIP and MCE Specific Headers
#include <e32base.h>
#include <e32std.h>
#include <e32math.h>
#include <s32mem.h>
#include <in_sock.h>
#include <badesca.h>
#include <e32cmn.h>
#include <mcesessionobserver.h>
#include <mcemanager.h>
#include <mcetransactiondatacontainer.h>
#include <mcemessagestream.h>
#include <mceexternalsource.h>
#include <mceexternalsink.h>
#include <mcemessagesink.h>
#include <mcemessagesource.h>
#include <mcemsrpsource.h>
#include <mcemsrpsink.h>
#include <mceoutsession.h>
#include <mceinsessionobserver.h>
#include <mceinsession.h>
#include <mceDataSinkObserver.h>
#include <qcontactmanager.h>


//Profile agent, SIP stack inclusions
#include <sipprofileregistryobserver.h>
#include <sipprofile.h>
#include <sip.h>
#include <sipobserver.h>

//#include "rcschatengine_globals.h"

class TMsrpChatSession;
class CContactItem;
class ChatContactManager;
class ContactView;



QTM_BEGIN_NAMESPACE
class QContact;
class QContactDetailFilterPrivate;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE 

const TInt KGranularity = 12;
const TInt KAcceptSize = 24;
/* Interface implementation of MChatContactIntf */
class ChatContactManagerImpl:   public QObject,
                                public MMceSessionObserver, 
                                public MSIPProfileRegistryObserver,  // for CSIPProfileRegistry
                                public MSIPObserver, // for CSIP
                                public MMceInSessionObserver, // for incoming sessions from MCE
                                public MMceDataSinkObserver
                      
{
 public:
	ChatContactManagerImpl(ChatContactManager *apCntManager);
	~ChatContactManagerImpl();
	
	
		
	/* Setup MCE Manager */		
	RcsIMLib::RcsChatId createChatSession(QContactLocalId contactId, QString initMsg);	
	void closeSession(RcsIMLib::RcsChatId sessID);
	void acceptIncomingSession(RcsIMLib::RcsChatId sessID);
	
	
	// From MMceSessionObserver
	void SessionStateChanged(CMceSession& aSession, TMceTransactionDataContainer* aContainer);
	void SessionConnectionStateChanged(CMceSession& aSession, TBool aActive );
	void Failed( CMceSession& aSession, TInt aError );
    void UpdateFailed(CMceSession& aSession, TMceTransactionDataContainer* aContainer );
    
	// From MSIPProfileAgentObserver
    void ProfileRegistryEventOccurred(TUint32 aProfileId, TEvent aEvent);
	void ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError);
	        
	// From MSIPObserver
    void IncomingRequest(TUint32 aIapId, CSIPServerTransaction* aTransaction);
    void TimedOut(CSIPServerTransaction& aTransaction); 

    // From MMceInSessionObserver
    void IncomingSession(CMceInSession* aSession, TMceTransactionDataContainer* aContainer );
	void IncomingUpdate(CMceSession& aOrigSession, CMceInSession* aUpdatedSession, TMceTransactionDataContainer* aContainer );	        
	void DataReceived(CMceMediaStream& aStream,CMceMediaSink& aSink, const TDesC8& aData);

 private:
     void setupMceManagerL();
     RcsIMLib::RcsChatId createChatSessionL(QContactLocalId contactId, QString initMsg);
     void GetStreams(CMceSession& aSession, CMceMessageStream*& aUplinkStr, CMceMessageStream*& aDownlinkStr );
	
 private:     
	//Variables here
	QContactManager                    *mManager;
	ChatContactManager                 *mParent;    
	//SYM Specific Headers
	CMceManager                        *mMceMgr;
	CSIP                               *mSip;
	CSIPProfileRegistry                *mProfileReg;
	CSIPProfile                        *mProfile;
	RPointerArray<CMceSession>         mMceSessions;
	RPointerArray<TMsrpChatSession>    mChatSessionArray;
	TMceTransactionDataContainer       mDataContainer;
	RPointerArray<CSIPProfile>         mAllprofiles;
	//QList<QContact>                    mAllContacts;
	//QList<QContact>                    mAllSIPContacts;
	//QContactLocalId                    mMyID;
	//QContactLocalId                    mMyBuddyID;
	//QString                            mMyFirstName;
	//QContactDetailFilter               mFilter;
	HBufC8                             *mOriginatorUri;
	

};
#endif //__CHATCONTACT_MANAGER_IMPL_SYM_H__
