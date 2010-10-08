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



/* A set of interfaces to be implemented by providers for chat application.
 */

#ifndef __CHATINTERFACES_H__
#define __CHATINTERFACES_H__

#include <QObject>
#include <QString>
#include <QList>
#include <qcontactid.h>


QTM_BEGIN_NAMESPACE
class QContact;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

namespace RcsIMLib
{

typedef unsigned int RcsChatId;

/*
 * Manages creation of a new session, notification for incoming
 * session, closing the session. Also manages sending of
 * IM Message, and notification of incoming message,
 * and error notification.
 */
class MChatSessionIntf;

/* Manages fetching and Managing of contacts for
 * chat. The APIs allow for fetching of the list
* of contacts and also their presence information.
 * Also the status message can be filled up if supported.
 */
class MChatContactManagerIntf: public QObject
{
	Q_OBJECT
	public:
	/* Also APIs need to be there for setting status message,
	 * and presence info etc */

	virtual RcsChatId createChatSession(QContactLocalId contactId, QString initMsg)=0;
	virtual void closeSession(RcsChatId sessID)=0;
	virtual void acceptIncomingSession(RcsChatId sessID)=0;
	
signals:
    void incomingChatSession(QContactLocalId aMyBuddyID, QString incomingSipUri, QString initMsg, RcsIMLib::RcsChatId sessID);
    void sessionEstablised(RcsIMLib::RcsChatId sessID,  RcsIMLib::MChatSessionIntf* chatSession);
    void sessionTerminated(RcsIMLib::RcsChatId sessID,  RcsIMLib::MChatSessionIntf* chatSession);

};

/* The interface used to initiate a chat session with a 
 * particular contact.
 * The ChatSessionManager is symbolised by an instance of MChatInterface
 */
class MChatSessionIntf: public QObject
{
	Q_OBJECT
	public:

	/* Creates a chat session with the specified contact.
	 * If the session creation was successful the MChatSessionManagerIntf
	 * returned will be a valid pointer.
	 */
	virtual bool sendChatData(QString newChatData)=0;
    virtual bool endChatSession()=0;

	//Dirty APIs :(
	//Use this API to set any Platform Parameters
	virtual void setPlatformParams(void *apPlatFormParam)=0;
	//Get the Platform implementations
	virtual void* getPlatformImpl()=0;

	signals:
	void newChatData(QString newChatData);
	/* Notifies of error and the appropriate error
	 * string */
    void errorMessage(QString errorString);
	/* Additional info such as,
	 * User is typing a message etc,
	 * The same will be reset by giving an empty string
	 * when appropriate.*/
	void additionalInfo(QString additionalInfo);


};

} //namespace

#endif //__CHATINTERFACES_H__

