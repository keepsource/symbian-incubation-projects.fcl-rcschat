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


#include "rcsimmanagerfactory.h"
#include "chatinterfaces.h"
#include "chatcontactmanager.h"

using namespace RcsIMLib;

RcsIMLib::MChatContactManagerIntf* RcsIMManagerFactory::createIMManager()
{
	return new ChatContactManager();
}

QString RcsIMManagerFactory::protocol()
{
	return "rcsmsrp";
}

Q_EXPORT_PLUGIN2(symmsrpplugin, RcsIMManagerFactory)

//static const char *qt_plugin_verification_data = \
//      "pattern=""QT_PLUGIN_VERIFICATION_DATA""\n" \
//      "version=""4.6.3""\n" \
//      "debug=""false""\n" \
//      "buildkey=""Symbian full-config"; \
//            extern "C" __declspec(dllexport) \
//            const char *  qt_plugin_query_verification_data() \
//            { return qt_plugin_verification_data; } \
//            extern "C" __declspec(dllexport) ::QObject *  qt_plugin_instance() \
//            { \
//            static QPointer<QObject> _instance; \
//            if (!_instance)      \
//                _instance = new RcsIMManagerFactory; \
//            return _instance; \
//        }

