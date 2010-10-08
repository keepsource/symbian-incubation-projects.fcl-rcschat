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


#include "chatinterfaces.h"
#include "immanagerfactory.h"
#include "chatimplfactory.h"
#include <QtPlugin>
#include <QDir>
#include <QLibraryInfo>
#include <QPluginLoader>
#include <QCoreApplication>

RcsIMLib::MChatContactManagerIntf* ChatImplFactory::createIMManager(QString protocol)
{
#ifdef Q_OS_SYMBIAN
    QDir pluginsDir = QLibraryInfo::location(QLibraryInfo::PluginsPath) + "/IMLIB";
#else
    QDir pluginsDir = QCoreApplication::applicationDirPath() + "/resource/qt/plugins/IMLIB";
#endif
	/* Now doing a dirty implementation.
	 * The real thing to do is parse manifests and compare each plugin for
	 * the protocol string 
	 */
	IMManagerFactory* imManagerFact=0;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) 
	{
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) 
	    {
           imManagerFact = qobject_cast<IMManagerFactory *>(plugin);
           if (imManagerFact && imManagerFact->protocol()==protocol)
		   {
		   RcsIMLib::MChatContactManagerIntf* imManager = imManagerFact->createIMManager();
			   //delete imManagerFact;
               return imManager;
		   }
		   //delete imManagerFact;
		   imManagerFact = 0;
        }	
	}
	return 0;
}

