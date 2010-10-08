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


#ifndef __RCS_IMMANAGER_FACTORY_H__
#define __RCS_IMMANAGER_FACTORY_H__

#include "immanagerfactory.h"

class RcsIMManagerFactory: public QObject, public IMManagerFactory
{

	Q_INTERFACES(IMManagerFactory)
	public:
		virtual RcsIMLib::MChatContactManagerIntf* createIMManager();
		virtual QString protocol();
};
#endif
