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



#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <qcontactmanager.h>
#include <qcontact.h>
#include <qcontactdetails.h>
#include "chatsession.h"
#include "chatcontactmanager.h"
#include "chatcontactmanagerimpl_sym.h"
#include "chatsessionimpl_sym.h"
#include "msrpchatsessiondata_sym.h"
#include <chatinterfaces.h>
#include <sipprofileregistry.h>
#include <mcesession.h>
#include "qcontactfilter.h"

using namespace RcsIMLib;


ChatContactManagerImpl::ChatContactManagerImpl(ChatContactManager *apCntManager):mParent(apCntManager),mProfile(NULL)
{
    QT_TRAP_THROWING(setupMceManagerL());
    mManager = new QContactManager("symbian");
	
	QContact  self = mManager->contact(mManager->selfContactId());
	//mMyID = self.localId();
	
	// To get Self SIP Uri.
	QContactDetail selfContactDetail = self.detail(QContactOnlineAccount::DefinitionName);
	const QContactOnlineAccount &onlineUrl = static_cast<const QContactOnlineAccount &>(selfContactDetail);
	QString selfSipUri = onlineUrl.accountUri();
	TPtrC16 dataPtr(reinterpret_cast<const TUint16*>(selfSipUri.utf16()));
	mOriginatorUri = HBufC8::NewL(dataPtr.Length());
	mOriginatorUri->Des().Copy(dataPtr);
			
}


ChatContactManagerImpl::~ChatContactManagerImpl()
    {

    for(int i=0;i<mMceSessions.Count();i++)
        {
        
        int mSessionState =  mMceSessions[i]->State();
        
        if (mSessionState <= CMceSession::EEstablished)
            mMceSessions[i]->TerminateL(0,0,0);
             
        }
    
    mMceSessions.ResetAndDestroy();//deletes streams, sources and sinks
    mMceSessions.Close();
    mChatSessionArray.ResetAndDestroy();
    mChatSessionArray.Close();
    delete mManager;
    delete mMceMgr;
    
    //delete mProfile;
    mAllprofiles.ResetAndDestroy();
    mAllprofiles.Close();
    
    delete mProfileReg;
    delete mSip;
    delete mOriginatorUri;
    
    }

void ChatContactManagerImpl::closeSession(RcsChatId sessID)
{
    
    CMceSession *currentSession = reinterpret_cast<CMceSession*>(sessID);
    
    int mSessionState =  currentSession->State();
           
    if (mSessionState == CMceSession::EEstablished)
        currentSession->TerminateL();
    
    //remove and close
    
    TInt idx = mMceSessions.Find(currentSession);
    if (idx != KErrNotFound)
        {
        delete currentSession;
        mMceSessions.Remove(idx);    
        }
    
    for(int i=0;i<mChatSessionArray.Count();i++)
        {
           if(mChatSessionArray[i]->iMceSession == currentSession)
               {
               delete mChatSessionArray[i];
               mChatSessionArray.Remove(i);
               break;
               }
        }
 }

void ChatContactManagerImpl::acceptIncomingSession(RcsChatId sessID)
{    
    CMceInSession *currentSession = reinterpret_cast<CMceInSession*>(sessID);
     if (currentSession->State() == CMceSession::EProceeding)
      {
         currentSession->AcceptL();
      }           
 }



RcsIMLib::RcsChatId ChatContactManagerImpl::createChatSession(QContactLocalId contactId, QString initMsg)
{
    RcsIMLib::RcsChatId id = 0;
    QT_TRAP_THROWING(id = createChatSessionL(contactId, initMsg));
    //QT_TRYCATCH_ERROR(id = createChatSessionL(contactId, initMsg));
    return id;
}


RcsIMLib::RcsChatId ChatContactManagerImpl::createChatSessionL(QContactLocalId contactId, QString initMsg)
{             
    // To get recipient SIP Uri.
    const QContact buddy = mManager->contact(contactId);
    QContactDetail buddyContactDetail = buddy.detail(QContactOnlineAccount::DefinitionName);
    const QContactOnlineAccount &onlineUrl = static_cast<const QContactOnlineAccount &>(buddyContactDetail);
    QString buddySipUri = onlineUrl.accountUri();
    TPtrC16 dataPtr(reinterpret_cast<const TUint16*>(buddySipUri.utf16()));
    HBufC8 *recipientUri = HBufC8::NewL(dataPtr.Length());
    CleanupStack::PushL(recipientUri);
    recipientUri->Des().Copy(dataPtr);
    
    CMceOutSession *outSess ;
    if(mProfile)
        {
        outSess = CMceOutSession::NewL(*mMceMgr, *mProfile, *recipientUri);
        }
    else
        {        
        outSess = CMceOutSession::NewL(*mMceMgr, 1,*mOriginatorUri, *recipientUri); 
        }        
    CleanupStack::PopAndDestroy(recipientUri);
    CleanupStack::PushL(outSess);
    
    /* Add the out session to the list of MCESessions */
    mMceSessions.Append(outSess);
    CleanupStack::Pop(outSess);

    /* Streams Creation */
    // Create the stream for UpLink and Downlink
    CMceMessageStream* msgStrmUp = CMceMessageStream::NewL();
    CleanupStack::PushL(msgStrmUp);
    
    // Message Source
    CMceMessageSource* messageSrc = CMceMessageSource::NewL(*mMceMgr);  
    CleanupStack::PushL(messageSrc);  
    
    //MSRP Sink
    CMceMsrpSink* msrpSink = CMceMsrpSink::NewL();
    CleanupStack::Pop(messageSrc);
    CleanupStack::PushL(msrpSink);
    CleanupStack::PushL(messageSrc);
    msgStrmUp->SetSourceL(messageSrc);
    CleanupStack::Pop(messageSrc);

    // Add sink  to the stream
    msgStrmUp->AddSinkL(msrpSink);
    CleanupStack::Pop(msrpSink);
    msgStrmUp->ConnectionSetUpL(CMceMessageStream::EActive);

    // Set AcceptTypes
    CDesC8ArrayFlat *acceptTypes = new(ELeave)CDesC8ArrayFlat(KGranularity);
    _LIT8(KtextPlain,"text/plain");   
    acceptTypes->AppendL(KtextPlain);    

    msrpSink->SetAcceptTypesL(*acceptTypes);
    delete acceptTypes;

    _LIT8(KAcceptWrap, "acceptwrap");
    TBuf8<KAcceptSize> accept(KAcceptWrap);
    msrpSink->SetAcceptWrappedTypesL(accept);


    //outSess->AddStreamL(msgStrmUp);
    CleanupStack::Pop(msgStrmUp);


    // Create the stream for Downlink
    CMceMessageStream *msgStrmDown = CMceMessageStream::NewL();

    // MSRP Source
    CMceMsrpSource *msrpSrc = CMceMsrpSource::NewL();

    // Message Sink
    CMceMessageSink* messageSink = CMceMessageSink::NewL(*mMceMgr);
    CleanupStack::PushL(msrpSrc);
    msgStrmDown->SetSourceL(msrpSrc);
    CleanupStack::Pop(msrpSrc);

    // Add sink  to the stream
    CleanupStack::PushL(messageSink);
    msgStrmDown->AddSinkL(messageSink);
    CleanupStack::Pop(messageSink);
    msgStrmDown->ConnectionSetUpL(CMceMessageStream::EActive);

    CleanupStack::PushL(msgStrmUp);
    msgStrmDown->BindL(msgStrmUp);
    CleanupStack::Pop(msgStrmUp);

    CleanupStack::PushL(msgStrmDown);
    outSess->AddStreamL(msgStrmDown);
    CleanupStack::Pop(msgStrmDown);
    
    /* A new Chat Session with the Platform specific Init Params */
    ChatSession *chatSession = new ChatSession();

    /* Create the ChatSession here */
    TMsrpChatSession *msrpChatSession = new TMsrpChatSession();
    msrpChatSession->iMceManager = mMceMgr;
    msrpChatSession->iMceSession = outSess;
    msrpChatSession->iChatSession = chatSession;
    msrpChatSession->iStream1 = msgStrmUp;
    msrpChatSession->iStream2 = msgStrmDown;

    //chatSession->setPlatformParams(msrpChatSession);

    mChatSessionArray.Append(msrpChatSession);


    //Start the session establishment here. Emit the signal when everything is
    //done. The session will be matched my the ID returned.
    //RcsChatId sessID = (RcsChatId)&outSess;
    
    
    //Create the CDesC8Array with the syntax of the subject header and pass it 
    //such that the INVITE is constructed with Subject header.
    
    CDesC8ArrayFlat *mSIPHeaders = new CDesC8ArrayFlat(20);
    QString mSubjectHeader = "Subject:"+ initMsg;
    TPtrC16 mDataPtr(reinterpret_cast<const TUint16*>(mSubjectHeader.utf16()));
    RBuf8 mChatData;
    mChatData.Create(300);
    mChatData.Copy(mDataPtr);
    
    mSIPHeaders->AppendL(mChatData);
    outSess->EstablishL(0,mSIPHeaders,0,0,0);                                                                                                                                                                                                                                                                                                                                                  

    return reinterpret_cast<RcsChatId>(outSess);
}

void ChatContactManagerImpl::setupMceManagerL()
{
    //RFileLogger::Write( KLogDir1, KLogFile1, EFileLoggingModeAppend, _L("setupMceManagerL ChkPt 0") );
    //Setup MCE For accepting and receiving chat messages
    mMceMgr = CMceManager::NewL(TUid::Uid(0x10009388), &mDataContainer);

    // Set session observer to get the session related state changes
    mMceMgr->SetSessionObserver(this);    

    // Set InSession Observer to recieve the incoming session
    mMceMgr->SetInSessionObserver(this);
    mMceMgr->SetDataSinkObserver(this);

    TUid appUid;
    
    appUid.iUid = 0xA00001EC; //TODO get APPUID Correct
    TRAPD(errsip, mSip = CSIP::NewL(appUid , *this))
    //TRAPD(errsip, mSip = CSIP::NewL(TUid::Null(), *this))
    if (!errsip) {
        //CleanupStack::PushL(sipClient);
        mProfileReg = CSIPProfileRegistry::NewL(*mSip, *this);
        //CleanupStack::Pop(mSip);
    }
    
    //assumes first profile is registered and valid    
    //CleanupStack::PushL(profileReg);
    //mProfile = profileReg->DefaultProfileL();
    
        
    //later check for tag matches
    
    
    mProfileReg->ProfilesL(mAllprofiles);
    if (mAllprofiles.Count())
    {     
        mProfile = mAllprofiles[0];
    }
    
    //CleanupStack::PopAndDestroy(profileReg);
    //CleanupStack::PopAndDestroy(sipClient);
}


void ChatContactManagerImpl::SessionStateChanged( CMceSession& aSession, TMceTransactionDataContainer* aContainer)
{
    if (aSession.State() == CMceSession::EEstablished)
    {       
        
        CMceMessageStream* downlinkStr = NULL;
        CMceMessageStream* uplinkStr = NULL;
        
        GetStreams(aSession, uplinkStr, downlinkStr );
                               
        if (uplinkStr && uplinkStr->Source() &&
                uplinkStr->Source()->Type() == KMceMessageSource)
            {
            uplinkStr->Source()->DisableL();
            if (!uplinkStr->Source()->IsEnabled())
                uplinkStr->Source()->EnableL();
            }
        
        if (downlinkStr && downlinkStr->Sinks().Count() > 0)
            {
            // assume only one sink is added
            downlinkStr->Sinks()[0]->DisableL();
            if (! downlinkStr->Sinks()[0]->IsEnabled() )
            downlinkStr->Sinks()[0]->EnableL();
            }
        
        for(int i=0;i<mChatSessionArray.Count();i++)
            {
               if(mChatSessionArray[i]->iMceSession == &aSession)
                   {
                   mChatSessionArray[i]->iStream1 = uplinkStr;
                   mChatSessionArray[i]->iStream2 = downlinkStr;
                   mChatSessionArray[i]->iChatSession->setPlatformParams(mChatSessionArray[i]);
                   break;
                   }
            }
        
       
/*  //can be terminated without establishment
        // A new Chat Session with the Platform specific Init Params 
        ChatSession *chatSession = new ChatSession();

         //Create the ChatSession here 
        TMsrpChatSession *msrpChatSession = new TMsrpChatSession();
        msrpChatSession->iMceManager = mMceMgr;
        msrpChatSession->iMceSession = &aSession;
        msrpChatSession->iChatSession = chatSession;
        msrpChatSession->iStream1 = uplinkStr;
        msrpChatSession->iStream2 = downlinkStr;

        chatSession->setPlatformParams(msrpChatSession);

        mChatSessionArray.Append(msrpChatSession);
*/

        /* Notify the UI that Session is established. Pointer to aSession
         * serves as the key */
        for(int i=0;i<mChatSessionArray.Count();i++)
            {
               if(mChatSessionArray[i]->iMceSession == &aSession)
                   {
                   emit mParent->sessionEstablised(reinterpret_cast<RcsChatId>(&aSession), mChatSessionArray[i]->iChatSession); 
                   break;
                   }
            }
        
        
    }
    else if (aSession.State() == CMceSession::EProceeding)
    {
    //get originator
    const TDesC8& remoteURI = aSession.Originator();
    const QString remoteSIPURI = QString::fromUtf8((const char*)remoteURI.Ptr(),remoteURI.Length());
    
    //Pass the remote sip uri as a filter and fetch the buddy name. 
    //Also emit a signal here for incomingChatSession with the contactID 

    // To fetch the buddy name using the sip URI.
    QList<QContactSortOrder> order;
    QStringList list;
    QContactDetailFilter filter;
    filter.setDetailDefinitionName(QContactOnlineAccount::DefinitionName, QContactOnlineAccount::SubTypeSip);
    filter.setValue(remoteSIPURI);
    
    QList<QContact> allContacts;
    allContacts = mManager->contacts(filter);
    unsigned int buddyID = 0;
    if(allContacts.count())
        {
        buddyID = allContacts[0].localId();
        }
  
    QString imMsg;
	

    for(int i=0;i<mChatSessionArray.Count();i++)
    {
       if(mChatSessionArray[i]->iMceSession == &aSession)
           {
           imMsg = mChatSessionArray[i]->iInitMsg;
           break;
           }
    }
    
     
    //emit on SSC to proceeding as a result of call to updateL //see insession header
    //emit after update or could lead to accept call before update, little chance as user interaction involved
    emit mParent->incomingChatSession(buddyID, remoteSIPURI, imMsg, reinterpret_cast<RcsChatId>(&aSession));
    }
    else if (aSession.State() == CMceSession::ETerminated)
    {
        for(int i=0;i<mChatSessionArray.Count();i++)
        {
           if(mChatSessionArray[i]->iMceSession == &aSession)
               {
               emit mParent->sessionTerminated(reinterpret_cast<RcsChatId>(&aSession), mChatSessionArray[i]->iChatSession);
               break;
               }
        }
    
    }
}



void ChatContactManagerImpl::SessionConnectionStateChanged( CMceSession& , TBool /*aActive*/ )
{
}

void ChatContactManagerImpl::Failed( CMceSession& /*aSession*/, TInt /*aError*/ )
{
}

void ChatContactManagerImpl::UpdateFailed(CMceSession& /*aSession*/, TMceTransactionDataContainer* /*aContainer*/ )
{
}

// From MSIPProfileAgentObserver
void ChatContactManagerImpl::ProfileRegistryEventOccurred(TUint32 /*aProfileId*/, TEvent /*aEvent*/)
{
}

void ChatContactManagerImpl::ProfileRegistryErrorOccurred(TUint32 /*aProfileId*/,  TInt /*aError*/)
{
}

// From MSIPObserver
void ChatContactManagerImpl::IncomingRequest(TUint32 /*aIapId*/, CSIPServerTransaction* /*aTransaction*/)
{
}

void ChatContactManagerImpl::TimedOut(CSIPServerTransaction& /*aTransaction*/)
{
}

// From MMceInSessionObserver
void ChatContactManagerImpl::IncomingSession(CMceInSession* aSession, TMceTransactionDataContainer* aContainer )
{       
    if (aSession->State() == CMceSession::EIncoming)
    {
        ///////////////////////
        QString imMsg;
        //Fetch the message from the SIP INVITE
        CDesC8Array *mSIPHeader = aContainer->GetHeaders(); 
        TInt count = mSIPHeader->MdcaCount();
           
        for (int i=0; i<count; i++)
        {
            TPtrC8 data = mSIPHeader->MdcaPoint(i);
            QString header = QString::fromUtf8((const char*)data.Ptr(),data.Length());
            if (header.contains("Subject",Qt::CaseSensitive))
                {
                    QChar sep = ':';
                    //QString mChatData = header.section(sep,1);
                    imMsg = header.section(sep,1);
                }
                else 
                {
                    //QString mChatData = "";
                    imMsg = "";
                }
        }
        //////////////////////
    
        // get the stream for this incoming session to set the MSRP capabilities
        CMceMessageStream* downlinkStr = NULL;
        CMceMessageStream* uplinkStr = NULL;
        GetStreams(*aSession, uplinkStr, downlinkStr);
        
        ChatSession *chatSession = new ChatSession();

        // Create the ChatSession here 
        TMsrpChatSession *msrpChatSession = new TMsrpChatSession();
        msrpChatSession->iMceManager = mMceMgr;
        msrpChatSession->iMceSession = aSession;
        msrpChatSession->iChatSession = chatSession;
        msrpChatSession->iStream1 = uplinkStr;
        msrpChatSession->iStream2 = downlinkStr;
        msrpChatSession->iInitMsg = imMsg;

        mChatSessionArray.Append(msrpChatSession);
        
        
         if (downlinkStr )
         {
            CMceMsrpSource* msrpSource = static_cast<CMceMsrpSource*> (downlinkStr->Source());
            if(msrpSource)
             {
                downlinkStr->ConnectionSetUpL(CMceMessageStream::EPassive);
                //uplinkStr->ConnectionSetUpL(CMceMessageStream::EPassive);

                // Set AcceptTypes
                CDesC8ArrayFlat *acceptTypes = new(ELeave)CDesC8ArrayFlat(KGranularity);
                _LIT8(KtextPlain,"text/plain");
                _LIT8(KMessageCpim, "message/cpim");
                acceptTypes->AppendL(KtextPlain);
                acceptTypes->AppendL(KMessageCpim);

                msrpSource->SetAcceptTypesL(*acceptTypes);
                delete acceptTypes;

                _LIT8(KAcceptWrap, "acceptwrap");
               TBuf8<KAcceptSize> accept(KAcceptWrap);
               msrpSource->SetAcceptWrappedTypesL(accept);
             }
          }
         
         /* Add the out session to the list of MCESessions */
         mMceSessions.Append(aSession);
         aSession->UpdateL();
       }         
}


void ChatContactManagerImpl::IncomingUpdate(CMceSession& , CMceInSession* , TMceTransactionDataContainer* )
{
}

void ChatContactManagerImpl::DataReceived(CMceMediaStream& aStream, CMceMediaSink& /*aSink*/, const TDesC8& aData)
{
    /* Find the Session for which this Message is intented */
    for(int i=0; i<mChatSessionArray.Count(); i++)
    {
       if(mChatSessionArray[i]->iStream1 == &aStream || mChatSessionArray[i]->iStream2 == &aStream)
       {
           ChatSessionImpl *pImpl = static_cast<ChatSessionImpl*>(mChatSessionArray[i]->iChatSession->getPlatformImpl());
           pImpl->incomingData(aData);
       }
    }
}


void ChatContactManagerImpl::GetStreams(CMceSession& aSession, CMceMessageStream*& aUplinkStr, CMceMessageStream*& aDownlinkStr )
    {
    RPointerArray<CMceMediaStream> streams = aSession.Streams();
    for (TInt i=0; i<streams.Count();i++)
       {
       if (streams[i]->Type() == KMceMessage  && 
               streams[i]->Source() &&
               streams[i]->Source()->Type() == KMceMSRPSource)
           {
           aDownlinkStr =static_cast<CMceMessageStream*> (streams[i]);
           //break;
           }
           
       if (streams[i]->Type() == KMceMessage  && 
               streams[i]->Source() &&
               streams[i]->Source()->Type() == KMceMessageSource)
           {
           aUplinkStr =static_cast<CMceMessageStream*> (streams[i]);
           //break;
           }  
       if (aDownlinkStr == NULL && streams[i]->BoundStream())
           {
           if (streams[i]->BoundStreamL().Type() == KMceMessage  && 
                   streams[i]->BoundStreamL().Source() &&
                   streams[i]->BoundStreamL().Source()->Type() == KMceMSRPSource)
               {
               CMceMessageStream& stream =static_cast<CMceMessageStream&> (streams[i]->BoundStreamL());
               aDownlinkStr =  &stream;
             //  break;
               } 
           }
           
       if (aUplinkStr == NULL && streams[i]->BoundStream())
           {
           if (streams[i]->BoundStreamL().Type() == KMceMessage  && 
                   streams[i]->BoundStreamL().Source() &&
                   streams[i]->BoundStreamL().Source()->Type() == KMceMessageSource)
               {
               CMceMessageStream& stream =static_cast<CMceMessageStream&> (streams[i]->BoundStreamL());
               aUplinkStr =  &stream;
               } 
           }
       }   // for loop

    }



