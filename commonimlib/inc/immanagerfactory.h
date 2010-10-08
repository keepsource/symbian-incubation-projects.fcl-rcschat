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


#ifndef __IMMANAGER_FACTORY__
#define __IMMANAGER_FACTORY__

#include <QString>
#include <QtPlugin>

namespace RcsIMLib
{
	class MChatContactManagerIntf;
}

class IMManagerFactory
{
	public:
		virtual RcsIMLib::MChatContactManagerIntf* createIMManager()=0;
		virtual QString protocol()=0;
};

Q_DECLARE_INTERFACE(IMManagerFactory, "com.nokia.qt.Plugin.IMManagerFactory/1.0");

#endif
