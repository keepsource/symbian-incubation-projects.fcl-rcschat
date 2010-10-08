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
* RCS IM Library - Initial Version
*
*/

#ifndef __CHAT_IMPL_FACTORY__
#define __CHAT_IMPL_FACTORY__

#include "imlib_common.h"
#include <QString>

namespace RcsIMLib
{
	class MChatContactManagerIntf;
}

class COMMONIMLIB_EXPORT ChatImplFactory
{
	public:
	static RcsIMLib::MChatContactManagerIntf* createIMManager(QString protocol);
};


#endif
